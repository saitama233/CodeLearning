var rx, ry;
launchApp("微信读书");
//共循环300次，每次60秒，也就是共5个小时
for(var i = 1; i <= 300; i++) {
	sleep(60000);//貌似就是60秒
    rx = random(900, 1020);
    ry = random(600, 666);
    click(960, 600);//我的屏幕是1920*1080，纵坐标中间就是960，横坐标偏右就是600
}
exit();
