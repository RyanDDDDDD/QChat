const grpc = require("@grpc/grpc-js")
const message_proto = require("./proto")
const const_module = require("./const")
const emailModule = require("./email")
const redis_module = require("./redis")
const {v4: uuidv4} = require('uuid')

async function GetVerifyCode(call, callback) {
    console.log("email is ", call.request.email)
    try{
        let query_res = await redis_module.GetRedis(const_module.code_prefix+call.request.email)
        console.log("query_res is:", query_res)
        let uniqueId = query_res
        if (query_res === null) {
            uniqueId = uuidv4();
            if (uniqueId.length > 4) {
                uniqueId = uniqueId.substring(0, 4)
            }

            let bres = await redis_module.SetRedisExpire(const_module.code_prefix+call.request.email, uniqueId,600)
            if(!bres){
                callback(null, { email:  call.request.email,
                    error:const_module.Errors.RedisErr
                });
                return;
            }
        }

        console.log("uniqueId is ", uniqueId)
        let text_str =  'Your auth code: '+ uniqueId +' Plz register within 3 mins!!!'
        //send email
        let mailOptions = {
            from: 'ryanwu199806@gmail.com',
            to: call.request.email,
            subject: 'code',
            text: text_str,
        };
    
        let send_res = await emailModule.SendMail(mailOptions);
        console.log("send res is ", send_res)
        if (!send_res) {
            callback(null, { email:  call.request.email,
                error:const_module.Errors.Exception
            });

            return
        }

        callback(null, { email:  call.request.email,
            error:const_module.Errors.Success
        })
        
    }catch(error){
        console.log("catch error is ", error)

        callback(null, { email:  call.request.email,
            error:const_module.Errors.Exception
        }) 
    }
     
}

function main() {
    var server = new grpc.Server()
    server.addService(message_proto.VerifyService.service, { GetVerifyCode: GetVerifyCode })
    server.bindAsync('127.0.0.1:50051', grpc.ServerCredentials.createInsecure(), () => {
        // server.start()
        console.log('grpc server started')        
    })
}

main()