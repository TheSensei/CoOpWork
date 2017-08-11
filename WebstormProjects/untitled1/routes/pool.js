/**
 * Created by vsen on 4/6/17.
 */
var mysql = require('mysql');

//creating connection to db...
module.exports = mysql.createPool ( {
    connectionLimit: 100,
    host    :   '127.0.0.1',
    user    :   'root',
    password:   'root',
    database:   'ArresterWorks',
    debug   :    true
});