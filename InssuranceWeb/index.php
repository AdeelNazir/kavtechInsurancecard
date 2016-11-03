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
        <form>
            <!-- Portfolio Item Heading -->
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header"> 
                        <small>
                            <form>
                                <div class="form-group">
                                    <label>Upload Card</label>
                                    <input type="file" id="fileUploader" class="form-control" name="files[]" style="margin-top: 5px; width: 610px;" accept=".jpg, .png, .jpeg, .gif, .bmp, .tif, .tiff|images/*"/>
                                </div>
                            </form>
                        </small>
                    </h1>
                </div>
            </div>
            <!-- /.row -->

            <!-- Portfolio Item Row -->
            <div class="row">
                <div class="col-md-8" style="width: 640px; height: 480px; Border 1px solid #ccc">
                    <!-- <img id="img_1" class="img-responsive" src="http://placehold.it/640x480" alt="Ocr Image"> -->
                    
                    <canvas id="canvas_01" width="640" height="480" style="border:1px solid #d3d3d3; cursor: pointer">
                            Your browser does not support the HTML5 canvas tag.
                    </canvas>

                </div>
                <div class="col-md-4" style="border:1px dashed #d9534f; margin-left: 30px;">
                    <h2><span class="label label-warning">Pending List</span></h2>
                    <div style="height: 300px; overflow-y: auto;">
                        <ul style="list-style-type:none">
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">1</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;">2</span></span></span></h3></li>
                          <li><h3><span class="label label-danger">Pending <span class="badge" style="background-color: white; color: #d9534f;"><b>3</b></span></span></span></h3></li>
                        </ul>
                     </div>
                    <h2>
                            <a href="pendings.php"><button type="button" class="btn btn-primary">Process <span class="badge">Pendings</span></button></a>
                    </h2>   
                </div>

                <div class="col-md-12" style="margin-top: 10px;">
                    <button type="button" onclick="processCard();" id="btnProcessCard" class="btn btn-primary">Process <span class="badge"> OCR</span></button>
                </div>
            </div>
            <!-- /.row -->
        </form>

        <!-- Related Projects Row -->
        <div class="row">

            <div class="col-lg-12">
                <h3 class="page-header">OCR Result</h3>
            </div>

            <div class="col-lg-12">
                <p>
                    Here will be OCR result Text

                </p>


                <!-- <a href="#">
                    <img class="img-responsive portfolio-item" src="http://placehold.it/500x300" alt="">
                </a>
            </div>

            <div class="col-sm-3 col-xs-6">
                <a href="#">
                    <img class="img-responsive portfolio-item" src="http://placehold.it/500x300" alt="">
                </a>
            </div>

            <div class="col-sm-3 col-xs-6">
                <a href="#">
                    <img class="img-responsive portfolio-item" src="http://placehold.it/500x300" alt="">
                </a>
            </div>

            <div class="col-sm-3 col-xs-6">
                <a href="#">
                    <img class="img-responsive portfolio-item" src="http://placehold.it/500x300" alt="">
                </a> -->
            </div>

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
        var canvas, context, currImagePath, imageObj, ImageData;
        function init() 
        {
            canvas = document.getElementById("canvas_01");
            context = canvas.getContext("2d");

            var imageLoader = document.getElementById('fileUploader');
            imageLoader.addEventListener('change', handleFrontImage, false);
        }
        function handleFrontImage(e) 
        {
            debugger;
            file = e;
            var reader = new FileReader();
            reader.onload = function (event) {
                imageObj = new Image();
                context.beginPath();
                imageObj.onload = function () 
                {
                    
                    canvas.width = imageObj.width;
                    canvas.height = imageObj.height;
                    context.drawImage(imageObj,0,0);     
                    
                    var dataURL = canvas.toDataURL();
                    var data = dataURL.replace(/^data:image\/(png|jpg);base64,/, ""); 
                    ImageData=dataURL;
                }
                imageObj.src = event.target.result;  
                currImagePath = event.target.result;
            }
            reader.readAsDataURL(e.target.files[0]);
        }
        function processCard()
        {
            saveImage();
            $.ajax({
                type:"GET",
                url: 'process.php',
                data: { process: "doProcess"}
            });
        }
        function saveImage() 
        {
            var mode = 1;
            // alert(ImageData);
            $.ajax({
                type:"POST",
                url:"saveImage2.php",
                data:{
                    image: ImageData
                },
                success: function saved($data) {
                    alert("saved");
                }
            });
        }
        init();
    
    </script>

</body>

</html>
