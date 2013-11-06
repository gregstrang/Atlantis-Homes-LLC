// Rotate Images brought in with CCK file>image field.
var displayTime = 6000; //In MS
var displaySpeed = 2000; //In MS
var rotateParent = new Array();
var parentContainer;

$(document).ready(function() {
	
	// Gather Parent Information
	$("div.views-field-field-image-home-fid div.field-content > div").each(function (i) {
		// Before gathering classes, add counter.
		$(this).attr("class", $(this).attr("class").split(" ",1)+'-'+i)

		rotateParent[i] = new Object();
		rotateParent[i].myClass = $(this).attr("class").split(" ",1);
		rotateParent[i].myHeight = String(findPos(this)).split(",",4)[3];
		
		
		if(i == 0) {
			$("div.views-field-field-image-home-fid").attr('style', "height:"+rotateParent[i].myHeight+"px");	
			$(this).attr("style","position:absolute;");
		} else {
			$(this).attr("style", "display:none; position:absolute;");
		}
	});
	
	if(rotateParent.length >= 2) {
		if(rotateParent.length < 3) {displayTime = 8000;}
		Trotate = setTimeout(function(){rotateChild(0,1); parameter = null}, displayTime);
	}
	
	function rotateChild (closeItem,openItem) {
		clearTimeout(Trotate); //clearTimeout everytime we enter. New timeouts are set at the bottom of this function.
		currentParent = rotateParent[closeItem];
				
		if(openItem == Number(rotateParent.length-1)) {
			nextOpenItem = 0;
		}else{
			nextOpenItem = openItem+1;			
		}
		
		previousOpenItem = openItem-1;
		if (previousOpenItem == -1) {
			previousOpenItem = Number(rotateParent.length-1);
		}else if(previousOpenItem == -2) {
			previousOpenItem = Number(rotateParent.length-2);
		}
		
		
		/* Transition Primary Image */		
		adjustParentContainer(rotateParent[openItem].myHeight);
		$("."+rotateParent[openItem].myClass).fadeIn(displaySpeed);
		$("."+currentParent.myClass).fadeOut(displaySpeed);
		
		
		Trotate = setTimeout(function(){rotateChild(openItem,nextOpenItem); parameter = null}, displayTime);
	}
		
	function adjustParentContainer (height) {
		$("div.views-field-field-image-home-fid").animate({
			height: height+"px"
		}, displaySpeed/2);
	}
	
	function findPos(obj) {
		var curtop = curleft = curwidth = 0;
		if (obj.offsetParent) {
			curtop = obj.offsetTop
			curleft = obj.offsetLeft
			curwidth = obj.offsetWidth
			curheight = obj.offsetHeight
			while (obj = obj.offsetParent) {
				curtop += obj.offsetTop
				curleft += obj.offsetLeft
			}
		}
		return [curleft,curtop,curwidth,curheight];
	}
});