<!DOCTYPE html>
<html lang="en">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Inssurance Company</title>

    <!-- Bootstrap Core CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom CSS -->
    <link href="css/portfolio-item.css" rel="stylesheet">



    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
        <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
        <script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->

</head>

<body>

    <!-- Navigation -->
    <nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
        <div class="container">
            <!-- Brand and toggle get grouped for better mobile display -->
            <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="index.php">Home</a>
            </div>
            <!-- Collect the nav links, forms, and other content for toggling -->
            <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
                <ul class="nav navbar-nav">
                    <li>
                        <a href="pendings.php">Pendings</a>
                    </li>
                    <!-- <li>
                        <a href="#">Services</a>
                    </li>
                    <li>
                        <a href="#">Contact</a>
                    </li> -->
                </ul>
            </div>
            <!-- /.navbar-collapse -->
        </div>
        <!-- /.container -->
    </nav>

    <!-- Page Content -->
    <div class="container">

        <!-- Portfolio Item Heading -->
        <div class="row">
            <div class="col-lg-12">
                <h1 class="page-header">Process New Inrollments
                    <!-- <small><form><input type="file" id="file" name="files[]" onchange="readURL(this)"/></form></small> -->
                </h1>
            </div>
        </div>
        <!-- /.row -->

        <!-- Portfolio Item Row -->
        <div class="row">
            <div class="col-md-2">
                <h2><span class="label label-danger">Pending List</span></h2>
                <ul style="list-style-type:none;">
                  <div style="height: 400px; overflow-y: auto;">
                        <li>
                            <h3>
                                <a style="text-decoration: none;" href="#" id="1" onclick="changeImage(this.id);">
                                    <span class="label label-warning">Pending 
                                        <span class="badge">1</span>
                                    </span>
                                </a>
                            </h3>
                        </li>
                  </div>
                </ul>
                <h2>
                </h2>
            </div>
            <div class="col-md-6" style="width: 650px; border: 1px solid #ccc">
                <form action="saveImage.php">

                    <!-- <div style="margin-left: 50px; width: 500px; height: 500px; margin-top: 50px; ">
                        <canvas id="canvas_01" width="100%" height="100%" style="border:1px solid #d3d3d3;"></canvas>
                    </div> -->
                    <div class="form-group" id="canvas_Image" style="width: 640px; height: 480px; margin-left: -14px; border: 1px #ccc dashed; background-color: #ffffff; ">
                        <!-- <canvas id="canvas_01"> -->
                        <!-- <div style=""> -->
                        <canvas id="canvas_01" style="border:1px solid #d3d3d3;width:100%; height:100%"></canvas>
                        <!-- </div> -->
                        <!-- <img id="imgCanvas" class="img-responsive" src="images\pendings/1.png" alt="Ocr Image"> -->
                        <!-- </canvas> -->
                    </div>
                    <button type="button" style="float:right;" onclick="rotate();" class="btn btn-primary">Rotate <span class="badge"><span class="glyphicon glyphicon-repeat"></span> </span></button>
                    <div class="form-group" style="float:left; width: 60%;">
                        <input type="file" id="fileUploader" class="" name="Get File" accept=".jpg, .png, .jpeg, .gif, .bmp, .tif, .tiff|images/*" />
                        <!-- rotate -->
                    </div>

                    <div class="form-group" style="width: 60%;">
                        <input type="text" class="form-control" id="lblCompanyName" placeholder="Company Name" name="">
                    </div>
                    <div class="form-group">
                        <button type="button" onclick="saveImage();" class="btn btn-primary">Save <span class="badge"> Template</span></button>
                    </div>
                </form>
            </div>
            <div class="col-md-3" style="border: 1px solid #ccc;">
                <div class="col-sm-4" style=" margin-left: -10px;">
                    <h3 style="margin-left: -10px;">Group</h3>
                    <select id="ddgroup" style="margin-left: -15px; width: 85px;">
                    </select>
                    <p></p>
                    <input style="margin-left: -15px; width: 85px;" type="text" class="form-control" id="lblgroup" placeholder="Group" name="">
                    <p></p>
                    <button onclick="addgroup(true);" type="button" style="margin-left: -15px;" class="btn btn-success">Add <span class="badge"> + </span></button>
                </div>
                <div class="col-sm-4" style="margin-left: 5px;">
                    <h3 style="margin-left: -10px;">Id</h3>
                    <select id="ddid" style="margin-left: -15px; width: 85px;">
                    </select>
                    <p></p>
                    <input style="margin-left: -15px; width: 85px;" type="text" class="form-control" id="lblid" placeholder="Id" name="">
                    <p></p>
                    <button onclick="addid(true);" type="button" style="margin-left: -15px;" class="btn btn-success">Add <span class="badge"> + </span></button>
                </div>
                <div class="col-sm-4" style="margin-left: 5px; ">
                    <h3 style="margin-left: -10px;">Name</h3>
                    <select id="ddname" style="margin-left: -15px; width: 85px;">
                    </select>
                    <p></p>
                    <input style="margin-left: -15px; width: 85px;" type="text" class="form-control" id="lblname" placeholder="name" name="">
                    <p></p>
                    <button onclick="addname(true);" type="button" style="margin-left: -15px;" class="btn btn-success">Add <span class="badge"> + </span></button>
                </div>
                <div class="col-sm-12">
                    
                </div>
            </div>
        </div>
        <!-- /.row -->
        <!-- Related Projects Row -->

        <div class="row" style="margin-top: 200px;">

           <!--  <div class="col-lg-12">
                <h3 class="page-header">OCR Result</h3>
            </div>

            <div class="col-lg-12">
                <p>
                    Here will be OCR result Text

                </p>
            </div> -->

        </div>
        <!-- /.row -->

        <hr>

        <!-- Footer -->
        <footer>
            <div class="row">
                <div class="col-lg-12">
                    <p>Copyright &copy; kavtech 2016</p>
                </div>
            </div>
            <!-- /.row -->
        </footer>

    </div>
    <!-- /.container -->

    <!-- jQuery -->
    <script src="js/jquery.js"></script>

    <!-- Bootstrap Core JavaScript -->
    <script src="js/bootstrap.min.js"></script>
    <script type="text/javascript">

     var canvas, context, startX, endX, startY, endY, currImagePath, offsetX, offsetY, imageObj, ImageData, originalImageWidth, originalImageHeight, wf,hf, file;
        var mouseIsDown = 0;

        //readGroupTextFile("ocrTxtFiles/group.txt");
        $(document).ready(function readAllFiles() 
        {
            readGroupTextFile("ocrTxtFiles/group.txt",1);
            readGroupTextFile("ocrTxtFiles/id.txt",2);
            readGroupTextFile("ocrTxtFiles/name.txt",3);
        });




        function readGroupTextFile(file,num)
        {
            var rawFile = new XMLHttpRequest();
            rawFile.open("GET", file, false);
            rawFile.onreadystatechange = function ()
            {
                if(rawFile.readyState === 4)
                { 
                    if(rawFile.status === 200 || rawFile.status == 0)
                    {
                        var allText = rawFile.responseText;
                        var spliterrr = allText.split("\n");
                        

                        if (num == 1) 
                        {
                            var option = document.createElement("option");
                            option.text = "---";
                            option.value = "-1";
                            document.getElementById("ddgroup").options.add(option);
                        }
                        if (num == 2) 
                        {
                            var option = document.createElement("option");
                            option.text = "---";
                            option.value = "-1";
                            document.getElementById("ddid").options.add(option);
                        }
                        if (num == 3) 
                        {    
                            var option = document.createElement("option");
                            option.text = "---";
                            option.value = "-1";
                            document.getElementById("ddname").options.add(option);
                        }
                        
                        for (var i = 0; i < spliterrr.length; i++) {
                            if (spliterrr[i] != null && spliterrr[i] != '' && spliterrr[i] != ' ') 
                            {
                                if (num == 1) 
                                {
                                    document.getElementById("lblgroup").value = spliterrr[i];
                                    addgroup(false);
                                }
                                if (num == 2) 
                                {
                                    document.getElementById("lblid").value = spliterrr[i];
                                    addid(false);
                                }
                                if (num == 3) 
                                {    
                                    document.getElementById("lblname").value = spliterrr[i];
                                    addname(false);
                                }
                            }
                        }
                    }
                }
            }
            rawFile.send(null);
        }
       
        document.getElementById("lblname").value ='';
        document.getElementById("lblid").value ='';
        document.getElementById("lblgroup").value ='';
        function init() {
            canvas = document.getElementById("canvas_01");
            context = canvas.getContext("2d");

            var imageLoader = document.getElementById('fileUploader');
            imageLoader.addEventListener('change', handleFrontImage, false);
            canvas.addEventListener("mousedown", mouseDown, false);
            canvas.addEventListener("mousemove", mouseXY, false);
            canvas.addEventListener("mouseup", mouseUp, false);
        }

        function addgroup(editmode) 
        {
            var txt = document.getElementById("lblgroup").value;
            if (txt != null && txt != '' && txt != ' ')
            {
                var option = document.createElement("option");
                option.text = txt;
                option.value = txt;
                document.getElementById("ddgroup").options.add(option);
                document.getElementById("lblgroup").value = "";
                if (editmode) {writeTextToFile(txt,1);}
            }

            
        }
        function addid(editmode) 
        {
            var txt = document.getElementById("lblid").value;
            if (txt != null && txt != '' && txt != ' ')
            {
                var option = document.createElement("option");
                option.text = txt;
                option.value = txt;
                document.getElementById("ddid").options.add(option);
                document.getElementById("lblid").value='';
                if (editmode) {writeTextToFile(txt,2);}
            }
            
        }
        function addname(editmode) 
        {
            var txt = document.getElementById("lblname").value;
            if (txt != null && txt != '' && txt != ' ')
            {
                var option = document.createElement("option");
                option.text = txt;
                option.value = txt;
                document.getElementById("ddname").options.add(option);
                document.getElementById("lblname").value ='';
                if (editmode) {writeTextToFile(txt,3);}
            }
        }
        function writeTextToFile(txt,mode) 
        {
            jQuery.ajax({
            type: "GET",
            url: 'writeTextFile.php',
           
            data: {
                text: txt,
                mode: mode
            },
            success: function(data) {
                alert("Text has been added");
            }
            });
        }


        function handleFrontImage(e) 
        {
            
            file = e;
            var reader = new FileReader();
            reader.onload = function (event) {
                imageObj = new Image();
                context.beginPath();
                imageObj.onload = function () 
                {
                    
                    // imageObj.width = 640;
                    // imageObj.height = 480;
                    canvas.width = imageObj.width;
                    canvas.height = imageObj.height;
                    originalImageWidth = imageObj.width;
                    originalImageHeight = imageObj.height;
                    wf = originalImageWidth/640;
                    hf = originalImageHeight/480;
                    context.drawImage(imageObj,0,0);     
                    
                    var dataURL = canvas.toDataURL();
                    var data = dataURL.replace(/^data:image\/(png|jpg);base64,/, ""); 
                    ImageData=dataURL;
                    // console.log("Got Image");
                }
                imageObj.src = event.target.result;  
                currImagePath = event.target.result;
            }
            reader.readAsDataURL(e.target.files[0]);
        }

        function mouseUp(eve) {
            if (mouseIsDown !== 0) {
                mouseIsDown = 0;
                var pos = getMousePos(canvas, eve);
                endX = pos.x;
                endY = pos.y;
                drawSquare(); //update on mouse-up
                
                var x = startX*wf;
                var y = startY*hf;
                var w = endX - startX;
                var h = endY - startY;
                w = w* wf;
                h = h* hf;

                var dataURL = cropPlusExport(imageObj,x,y,w,h);
                var data = dataURL.replace(/^data:image\/(png|jpg);base64,/, ""); 
                //document.getElementById("hidden0").value = data;
                ImageData=dataURL;
            }
        }

        function cropPlusExport(img,cropX,cropY,cropWidth,cropHeight)
        {
          // create a temporary canvas sized to the cropped size
          var canvas1=document.createElement('canvas');
          var ctx1=canvas1.getContext('2d');
          canvas1.width=cropWidth;
          canvas1.height=cropHeight;
          // use the extended from of drawImage to draw the
          // cropped area to the temp canvas
          ctx1.drawImage(img,cropX,cropY,cropWidth,cropHeight,0,0,cropWidth,cropHeight);
          // return the .toDataURL of the temp canvas
          return(canvas1.toDataURL());
        }

        function mouseDown(eve) 
        {
            mouseIsDown = 1;
            var pos = getMousePos(canvas, eve);
            startX = endX = pos.x;
            startY = endY = pos.y;
            drawSquare(); //update
        }

        function mouseXY(eve) {

            if (mouseIsDown !== 0) {
                var pos = getMousePos(canvas, eve);
                endX = pos.x;
                endY = pos.y;

                drawSquare();
            }
        }

        function drawSquare() {
            // creating a square
            var x = startX*wf;
            var y = startY*hf;
            

            var w = endX - startX;
            var h = endY - startY;
            w = w* wf;
            h = h* hf;

            offsetX = (w < 0) ? w : 0;
            offsetY = (h < 0) ? h : 0;
            var width = Math.abs(w);
            var height = Math.abs(h);
            
            context.clearRect(0, 0, canvas.width, canvas.height);
            context.beginPath();
            context.drawImage(imageObj, 0,0);
            context.rect(x, y, w, h);
            context.lineWidth = 10;
            context.strokeStyle = 'black';
            context.stroke();
        }

        function getMousePos(canvas, evt) {
            var rect = canvas.getBoundingClientRect();
            return {
                x: evt.clientX - rect.left,
                y: evt.clientY - rect.top
            };
        }
///////////////////////////////////////////////////////////
        function saveImage()
        {
            var templName = document.getElementById("lblCompanyName").value;
            if (templName == null || templName == '') 
            {
                alert("Please Enter Company Name");
                return;

            }
            debugger;
            // alert(ImageData);
            jQuery.ajax({
            type: "POST",
            url: 'saveImage.php',
            data: {
                image: ImageData,
                companyName: templName
            },
            success: function(data) {
                alert("Template has been saved");
                document.getElementById("lblCompanyName").value = '';
            }
            });

        }
        function changeImage(id)
        {
            currImagePath = null;
            imageObj = new Image();
            context.beginPath();
            imageObj.onload = function() {
                canvas.width = imageObj.width;
                canvas.height = imageObj.height; 
                context.drawImage(imageObj, 0,0);
                originalImageWidth = imageObj.width;
                originalImageHeight = imageObj.height;
                wf = originalImageWidth/640;
                hf = originalImageHeight/480;
            };
            imageObj.src = "images/pendings/"+id+".png";
            currImagePath = "images/pendings/"+id+".png";
            document.getElementById("fileUploader").value = "";
        }
        function rotate()
        {
            // reader.onload = function (event) 
            // {
                // var img = new Image();
                // img.onload = function () 
                // {
                //alert(img.width+" "+img.height);
                canvas.width = imageObj.height;
                canvas.height = imageObj.width;
                context.clearRect(0,0,canvas.width,canvas.height);
                context.save();
                context.translate(canvas.width/2,canvas.height/2);
                context.rotate(90*Math.PI/180);
                context.drawImage(imageObj,-imageObj.width/2,-imageObj.height/2);
                originalImageWidth = imageObj.width;
                originalImageHeight = imageObj.height;
                wf = originalImageWidth/640;
                hf = originalImageHeight/480;
                context.restore();


                var dataURL = canvas.toDataURL();
                imageObj.src = dataURL;
                var data = dataURL.replace(/^data:image\/(png|jpg);base64,/, ""); 
                ImageData = dataURL;
                // var dataURL = canvas.toDataURL("image/png");
                // var data = dataURL.replace(/^data:image\/(png|jpg);base64,/, ""); 
                // document.getElementById("hidden0").value = data;
                
                // console.log("Got Image");
                // }
                // imageObj.src = event.target.result;  
            // }
            // reader.readAsDataURL(e.target.files[0]);
        }
        init();
    </script>
</body>

</html>
