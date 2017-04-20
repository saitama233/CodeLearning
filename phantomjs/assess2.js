var page = require('webpage').create();
var system = require('system');
var url = system.args[1];
//var url = 'http://www.baidu.com';
page.onConsoleMessage = function(msg){
    console.log(msg);
};

page.open(url, function(status){
    page.evaluate(function(){
        console.log(document.title);
    });
    phantom.exit();
});
