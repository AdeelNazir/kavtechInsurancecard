
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
   <head>
   <meta content="text/html;charset=utf-8" http-equiv="Content-Type">
<meta content="utf-8" http-equiv="encoding">
      <script Language="JavaScript" src="Scripts/jquery-3.1.0.min.js"></script>
	<script type="text/javascript" src="Scripts/vkbeautify.0.99.00.beta.js"></script>
      <title>TESTE</title>
   </head>
   <body>
   
		<input type="file" id="CardimageLoader" name="imageLoader"  style="Width:30%;height:100%" />
		<button type="button" id="btnrotate" style="display:none">Rotate</button>
		
   <select id="Age" onchange="changeAge()" style="display:none">
		<option value="0">OverAge</option>
		<option value="1">UnderAge</option>
		</select>
		<input type="button" onClick="detectMRZ()" value="Detect MRZ">
		<br/>
	    <canvas id="pictureBox1" style="border:solid red;width:50%;height:50%"></canvas>
		<p id="empDetails"></p>
<!--<div >
<div>
<label >Type:           </label>
<label id="Type"></label>

</div>
<div>
<label >Code:            </label>
<label id="Code"></label>

</div>
<div>
<label >FirstName:        </label>
<label id="FirstName"></label>

</div>
<div>
<label >LastName:         </label>
<label id="LastName"></label>

</div>
<div>
<label >Nationality:         </label>
<label id="Nationality"></label>

</div>
<div>
<label >Country Name:         </label>
<label id="CountryName"></label>

</div>

<div>
<label >PassportNumber:   </label>
<label id="PassportNumber"></label>

</div>
<div>
<label  >DOB:             </label>
<label id="DOB"></label>

</div>
<div>
<label >Sex:              </label>
<label id="Sex"></label>

</div>
<div>
<label >ExpiryDate:       </label>
<label id="ExpiryDate"></label>

</div>
<div>
<label >IDNumber:         </label>
<label id="IDNumber"></label>

</div>

<div>
<label >MRZ:             </label>
<textarea style="width:30%; height:30px" id="Line1"></textarea>

</div>

</div>-->


   </body>

   <script type="text/javascript">
   var file;
   var ImageData;
   
function changeAge()
{
document.getElementById("hiddenAge").value = document.getElementById("Age").value;
}
function loadImage(myData)
{ 
var jason=JSON.parse(myData);
jason=JSON.parse(jason);
console.log(jason);

var myDataArray=[jason.rectangle.x,jason.rectangle.y,jason.rectangle.w,jason.rectangle.h];
var line1=[jason.line1.x1,jason.line1.y1,jason.line1.x2,jason.line1.y2];
var line2=[jason.line2.x1,jason.line2.y1,jason.line2.x2,jason.line2.y2];
var arrayPoints=jason.listRectangle;
var len=jason.listRectangle.length;

document.getElementById('pictureBox1').value=jason.personInfo.Type;
var len=jason.listRectangle;
 



 var c = document.getElementById('pictureBox1');
    var ctx = c.getContext('2d');
		ctx.beginPath();
		ctx.rect(myDataArray[0]*c.width, myDataArray[1]*c.height, myDataArray[2]*c.width, myDataArray[3]*c.height);
		ctx.lineWidth = 1;
		ctx.strokeStyle = 'black';
		/*
		for (var a=0;a<len;a++){
		
	var lx=(arrayPoints[a].x*(jason.rectangle.w*c.width))+(jason.rectangle.x*c.width);
	var ly=(arrayPoints[a].y*(jason.rectangle.h*c.height))+(jason.rectangle.y*c.height);
	
	//ctx.rect(lx,ly , arrayPoints[a].w*(jason.rectangle.w*c.width), arrayPoints[a].h*(jason.rectangle.h*c.height));
	}
*/
	ctx.moveTo(line1[0]*c.width,line1[1]*c.height);
		ctx.lineTo(line1[2]*c.width,line1[3]*c.height);
		
		ctx.moveTo(line2[0]*c.width,line2[1]*c.height);
		ctx.lineTo(line2[2]*c.width,line2[3]*c.height);
		
		ctx.stroke();   


}
function handleFrontImage(e) {//"data:image/jpeg;base64,"
file = e;
    var c = document.getElementById('pictureBox1');
    var ctx = c.getContext('2d');
    var reader = new FileReader();
    reader.onload = function (event) {
        var img = new Image();
        img.onload = function () {
		//alert(img.width+" "+img.height);
            c.width = img.width;
            c.height = img.height;
            ctx.drawImage(img,0,0);		
            //ctx.drawImage(img, 0, 0);
			
            var dataURL = c.toDataURL();//c.toDataURL("image/png");
            var data = dataURL.replace(/^data:image\/(png|jpg);base64,/, ""); 
			//document.getElementById("hidden0").value = data;
			ImageData=dataURL;
			console.log("Got Image");
        }
        img.src = event.target.result;  
    }
    reader.readAsDataURL(e.target.files[0]);
	
	//document.getElementById('pictureBox1').value=jason.personInfo.Type;
//var len=jason.listRectangle;

}
function drawFun(myData){

if(myData.indexOf("Error")!=-1){
alert(myData);
}else{
loadImage(myData);
//alert("Valid MRZ");
}
}

function saveimage()
{
//alert("here");

jQuery.ajax({
    type: "POST",
    url: 'upload.php',
   
    data: {image: ImageData}
			});
}
function displayXml() {
	document.getElementById('empDetails').innerHTML = xmlString;
};

function htmlEntities(str) {
    var htmlString = String(str).replace(/&/g, '&amp;').replace(/</g,     '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;');
    return htmlString;
}
function detectMRZ(){
saveimage();
jQuery.ajax({
    type: "POST",
    url: 'MRZAPI.php',
   
    data: {functionname: 'getMRZFields'},

 success:function(xmlString) {
        //alert(xmlString); 
// Normal display of xml
//document.getElementById('empDetails').innerHTML = xmlString;
//alert("Normal display: \n" + xmlString);

// HTML conversion only
var htmlString = htmlEntities(xmlString);
document.getElementById('empDetails').innerHTML = htmlString;
//alert("HTML formatted display: \n" + htmlString);

// Using vkbeautify
var beautifyHtmlString = vkbeautify.xml(xmlString);
//document.getElementById('empDetails').innerHTML = beautifyHtmlString;
//alert("VKbeautify display: \n" + beautifyHtmlString);
         }
            
			
			});
}
function initialize() {

var imageLoader = document.getElementById('CardimageLoader');
    imageLoader.addEventListener('change', handleFrontImage, false);
	
}
 if (window.addEventListener) {
    window.addEventListener('load', initialize, false);
}
else if (window.attachEvent) {
    window.attachEvent('onload', initialize);
}
else {
    window.onload = initialize;
}
  </script>

</html>
