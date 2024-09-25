const mongoose = require('mongoose');
const RuleSchema = mongoose.Schema(
    {
        Rule: {
            type: String,
            required: [true]
        },
        type: {
            type: String,
            required: [true, 'Please enter Rule name'],
        },
        test :{
            type: String,
            required: [true]
        }
    },
    {
        timestamps: true
    }
) 
const Rules = mongoose.model('Rules', RuleSchema); 
module.exports = Rules;