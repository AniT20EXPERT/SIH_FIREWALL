const mongoose = require('mongoose');

const ips = mongoose.Schema(
    {
        ip_address : {
          ipv4 : {
            type: String,
            required: [true]
          },
          ipv6 : {
            type: String,
            required: [true]
          }
        },
        protocol : {
          tcp : {
            type: Boolean
          },
          udp : {
            type: Boolean
          }
        },
        domain_name : {
          type: String,
        }
      }
)
const appsSchema = mongoose.Schema(
    {
        app_exe_name: {
            type: String,
            required: [true, "enter application name!"]
        },
        app_exe_path :{
            type: String,
            required: [true, "enter application path!"]
        },
        app_ips : {
            type: [ips],
            required: [true, "set app traffic rules!"]
        }
    }
)
const RuleSchema = mongoose.Schema(
    {
        //the following schema item is required to select the rule document in collection
        type: {
            type: String,
            default: 'rule',
            immutable: true
        },
        
        //schema for the rest:-
        apps: {
            type:[appsSchema],
            required: [true],
            unique: [true]
        },
        

    },
    {
        timestamps: true
    }
) 
const Rules = mongoose.model('Rules', RuleSchema); 
module.exports = Rules;
