const nodemailer = require('nodemailer')
const config_module = require("./config")

/**
 * create proxy to send email
 */
let transport = nodemailer.createTransport({
    host: 'smtp.gmail.com',
    port: 465,
    secure: true,
    auth: {
        user: config_module.email_user, // sender email
        pass: config_module.email_pass  // email authentication code
    }
});

/**
 * function to send email
 * @param {*} mailOptions_ para
 * @returns 
 */
function SendMail(mailOptions_){
    return new Promise(function(resolve, reject){
        transport.sendMail(mailOptions_, function(error, info){
            if (error) {
                console.log(error);
                reject(error);
            } else {
                console.log('email sent successfuly: ' + info.response);
                resolve(info.response)
            }
        });
    })  
}

module.exports.SendMail = SendMail