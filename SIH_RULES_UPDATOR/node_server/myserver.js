// ctrl + s for applying changes to server when running with nodemon
const crypto = require('crypto');
const express = require('express')
const app = express()
app.use(express.json());

const Rules = require('./models/rules.model.js');
const mongoose = require('mongoose');

//connecting db

mongoose.connect('mongodb+srv://anirudhhadalaldpsv:<password>@mycluster.zqon3.mongodb.net/?retryWrites=true&w=majority&appName=mycluster')
.then(()=>{
    try{
        console.log("connected db succesfully");
    }
    catch (error) {
        // handle the error here
        console.error(error);
    }
});

//run server on port 3000

app.listen(3000, ()=>{
    console.log("server running on port 3000"); //only on console screen i.e terminal
});


function hashRequestBody(reqBody) {
    const hash = crypto.createHash('sha256');
    hash.update(JSON.stringify(reqBody));
    return hash.digest('hex');
  }



changes = {"changes":true};

app.get('/rules_body', async (req, res)=> {
    try {
        const get_rule = await Rules.findOne({ type: 'rule' });
        res.status(200).json(get_rule);
    } catch (error) {
        res.status(500).json({message : error.message});
    }
})

app.get('/rules_hash', async (req, res)=> {
    try {
        res.status(200).json({"rule_Hash" : rule_hash});
    } catch (error) {
        res.status(500).json({message : error.message});
    }
})


app.post('/addrule', async (req, res)=> {
    try{
        const rules = await Rules.create(req.body);
        res.status(200).json(rules);
        rule_hash = hashRequestBody(req.body); 
    }
    catch (error) {
        res.status(500).json({message : error.message});
    }
});

app.put('/changerule', async (req, res)=> {
    try{
        rule_hash = hashRequestBody(req.body);
        const update_rule = await Rules.findOneAndReplace({ type: 'rule' }, req.body);
        if(!update_rule) {
            return res.status(404).json({message : "Rule not found"});
        }
        const rule = await Rules.findOne({ type: 'rule' });
        res.status(200).json(rule);
        changes = {"changes":true};
        ;  
    }
    catch (error) {
        res.status(500).json({message : error.message});
    }
})

















