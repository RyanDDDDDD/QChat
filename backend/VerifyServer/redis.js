const config_module = require('./config')
const Redis = require("ioredis")

const RedisCli = new Redis({
    host: config_module.redis_host,
    port: config_module.redis_port,
    password: config_module.redis_passwd
})

RedisCli.on("error", function (err) {
    console.log("RedisCli connect error")
    
    RedisCli.quit()
})

//find value stored by key
async function GetRedis(key) {
    try {
        const result = await RedisCli.get(key)
        if (result === null) {
            console.log('Result:', '<'+result+'>', 'This key cannot be found...')
            return null
        }
        console.log('Result:', '<'+result+'>', 'Get key success!...')
        return result
    } catch(error) {
        console.log('GetRedis error is', error)
        return null
    }
}

//check if key exists
async function QueryRedis(key) {
    try{
        const result = await RedisCli.exists(key)
        if (result === 0) {
          console.log('result:<','<'+result+'>','This key is null...');
          return null
        }
        console.log('Result:','<'+result+'>','With this value!...');
        return result
    }catch(error){
        console.log('QueryRedis error is', error);
        return null
    }
}

//set key-value with expiration time
async function SetRedisExpire(key,value, exptime){
    try{
        // 设置键和值
        await RedisCli.set(key,value)
        // 设置过期时间（以秒为单位）
        await RedisCli.expire(key, exptime);
        return true;
    }catch(error){
        console.log('SetRedisExpire error is', error);
        return false;
    }
}

//quit redis client
function Quit(){
    RedisCli.quit();
}

module.exports = {GetRedis, QueryRedis, Quit, SetRedisExpire}
