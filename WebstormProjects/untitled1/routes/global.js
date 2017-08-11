/**
 * Created by vsen on 4/6/17.
 */
var mysql = require('mysql');

var pool = require('./pool');

exports.doQuery = function(query, callback) {
    pool.getConnection(function(err, connection) {
        if (err) {
            connection.release();
            return callback(err, 'Error connecting to database');
        }

        connection.query(query, function(err, rows){
            connection.release();
            if (err) {
                return callback(err, 'Error executing query');
            } else {
                //success
                return callback(err,rows);
            }
        });
    });
}