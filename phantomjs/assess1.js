var page = require('webpage').create();
var system = require('system');
var url = system.args[1];
//var url = 'http://www.baidu.com';

page.open(url, function(status){
    var title = page.evaluate(function(){
        return document.title;
    });
    console.log('Page tile is ' + title);
    phantom.exit();
});
