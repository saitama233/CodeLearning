//原作者貌似叫酷安@群主让我注册
//原版本脚本下载地址：https://github.com/start201711/autojs?files=1
//原版本软件下载地址：https://github.com/hyb1996/Auto.js/releases

//设备要求：需要root或者安卓7.0以上，以及autojs软件版本3.0版本以上才能使用
//1.准备工作：只需要一张小图片
//即：可以收取好友能量的时候，好友右上角那个绿色的小手图像，只要能包括绿色小手的图像就行，
//不能使用大的图像，命名为ant_forest_take1080.png，放在sdcard/autojs_script目录下
//建议第一次使用本脚本的时候，打开开发者模式的显示指针位置和触摸操作，可以直观的看到整个收取能量过程

//---------------------------
//20180407：以下为我修改的版本
//目前的问题，无法抓取"绿色能量"字样的能量
//20180415：已经可以抓取”绿色能量“字样的能量


var temp = images.read("/sdcard/autojs_script/ant_forest_take1080.png");
debug = false;
var debug_dir = "/sdcard/autojs_script/debug/ant_forest_take/";

main();
	
function main() {
	if (debug) {
		files.ensureDir(debug_dir);
	}
	
	handle_hand_picture();
	
	launch("com.eg.android.AlipayGphone");
	waitForPackage("com.eg.android.AlipayGphone");

	//获取截屏权限
	if (!requestScreenCapture()) {  
		toastLog("没有截图权限");  
		exit(); 
	}

	//不断的循环好友列表
	while(1) {
		get_energy();
	}
}

function handle_hand_picture() {
	if (!temp) {
		toastLog("缺少图片文件，请仔细查看\n使用方法的第一条！！！");
		switch (device.width) {
			case 1080:
				temp = images.load("https://raw.githubusercontent.com/start201711/autojs/master/take.png");
				break;
			case 720:
				temp = images.load("https://raw.githubusercontent.com/start201711/autojs/master/take720p.png");
				break;
			default:
				temp = null;
				break;
		}

		if (!temp) {
			toastLog("尝试下载take.png失败,脚本停止运行");
			exit();
		}
		toastLog("现在将尝试使用别人的图片，分辨率可能不匹配，脚本可能无法正常执行");
	} else {
		if (debug) {
			toastLog("读取图片成功");
		}
	}
}

function get_energy() {
	clickforest();

	className("android.widget.Button").desc("攻略").waitFor();
	toastLog("成功进入蚂蚁森林");
	sleep(3000);

	//点击查看更多好友
	clickmore();
	//收取所有好友能量
	takeothers();
	toastLog("结束收取好友能量");
	//收取完返回，相当于按两下左上角的"<-"
	idContains("h5_tv_nav_back").click();
	sleep(2000);
	idContains("h5_tv_nav_back").click();
	sleep(1000);
}

function clickforest() {
	var b = text("蚂蚁森林").findOne().bounds();
	var a = idContains("home_app_view").untilFind().filter(o => {
		return o.bounds().contains(b);
	});
	toastLog("点击蚂蚁森林");
	toastLog(a[0]);
	//没点到就不断的点击
	while (!a[0].click());    
}

function clickmore() {
	//下划两次
	swipe(540, 1500, device.width / 2, 100, 500);
	swipe(540, 1500, device.width / 2, 100, 500);

	//sleep(3000);
	sleep(1000);
	//点击查看更多好友
	press(device.width / 2, 1500, 1);
}

function takeothers() {
	while (1) {    //这个while(1)感觉可以去掉呢？？？
		//遍历当前页所有的小手图标
		for (var p = findImage(captureScreen(), temp); p; p = findImage(captureScreen(), temp)) {
			if (debug) {
				toastLog("进入好友的森林");
			}
			//进入有小手图标的好友
			press(device.width / 2, p.y + 30, 100);
			//收取能量
			take();
			//sleep(1000);
			sleep(500);
			//返回上一层
			idContains("h5_tv_nav_back").click();
			//sleep(2000);
			sleep(500)
		}
		if (debug) {
			images.captureScreen(debug_dir + new Date().getTime() + ".png");
		}

		if (descContains("没有更多了").find().size() > 0) {
			toastLog("找到\"没有更多了\"");
			if (descContains("没有更多了").findOne().bounds().top < device.height) {
				toastLog("break跳出while，结束takeothers()");
				break;
			}
		}
		//当前页没有小手图标便下滑
		swipe(device.width / 2, device.height * 4 / 5, device.width / 2, device.height * 1 / 5, 500);

		//sleep(2000);
		sleep(1000);
	}	
}



function take() {
	//找到右下角
	var right_bottom = className("android.widget.Button").desc("浇水").findOne();
	log(right_bottom);
	//找到左上角
	var left_top = descContains("返回").findOne();
	log(left_top);

	var energys = [];
	//确定上下左右的范围，也就是能量球所在的范围
	var left = 0;
	var right = device.width;
	var top = left_top.bounds().bottom;
	var bottom = right_bottom.bounds().top;

	//把范围内的所有能量球找出并push进energys数组
	log("left = " + left + "," + "top = " + top + "," + "right = " + right + "," + "bottom = " + bottom);
	//sleep(2000);
	sleep(300);
	//寻找"数字+g"或者"绿色能量"的图标
	//发现找不到绿色能量字样
	var all = descMatches("^((绿色|收集)能量|\\d+g)$").boundsInside(left, top, right, bottom).untilFind();
	toastLog("找到" + (all.size() - 1) + "个能量球");
	all.each(function(x) {
		energys.push(x);
	});

	energys.sort(function(o1, o2) {
		return distance(o1) - distance(o2);
	});

	if (energys.length > 0) {
		energys.splice(0, 1);
	}
	if (debug) {
		images.captureScreen(debug_dir + new Date().getTime() + ".png");
	}
	//之前怎么都点不到，打开开发者选项的显示指针才发现点的位置不对，然后发现x和y的坐标写反了
	for (var i = 0; i < energys.length; i++) {
		press(energys[i].bounds().left + 30, energys[i].bounds().top + 30, 1000);
		//sleep(1000);
		sleep(100);
		toastLog("点击->" + energys[i].bounds().top + "," + energys[i].bounds().left);
	}

	function distance(o) {
		return Math.pow((o.bounds().top - top), 2) + Math.pow((o.bounds().right - right), 2);
	}
}