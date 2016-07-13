'require strict'

var i2c = require('i2c-bus');
var devAddress = 0x1d;

var wire = i2c.openSync(2);

/*
var addressArray = wire.scan();
console.log("Lengde: " + addressArray.length);
for (var i=0 ; i<5; ++i) {
    console.log("address: " + addressArray[i]);
}
*/

var data = wire.readByteSync(devAddress, 0x01);
console.log(data);

