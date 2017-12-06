var page = require('webpage').create(),
    system = require('system'),
    t,address;

if(system.args.length === 1){
    console.log('Usage:loadspeed.js <some URL>');
    phantom.exit();
}

//console.log('1');

t = Date.now();
address = system.args[1];
console.log('address:' + address);
page.open(address,function(status){
    if(status !== 'success'){
        console.log('FAIL to load the address');
    }else{
        t = Date.now() - t;
        console.log('loading' + system.args[1]);
        console.log('Loading time' + t + 'msec');
    }
    phantom.exit();
});
