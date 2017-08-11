/**
 * Created by vsen on 1/12/17.
 */
/*jshint multistr:true */

var text = "REDACTED REDACTED REDACTED REDACTED REDACTED REDACTED REDACTED ALIENS REDACTED ALIENS REDACTED REDACTED ALIENS"

var myName = "Aliens";
var nameExists = false;

hits = [];

for (var i = 0; i < text.length; i++) {
    if (myName[0] === text[i]) {
        for (var j = 1; j < i + myName.length; j++) {
            if (myName[j] === text[j]) {
                console.log ("Am I reaching here?");
                nameExists = true;
            }
        }

    }

}


