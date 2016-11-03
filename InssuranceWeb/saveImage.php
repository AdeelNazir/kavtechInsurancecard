<?php
if (isset($_POST["image"]) && !empty($_POST["image"])) 
{
    // if (isset($_POST["mode"])) 
    // {
        
    // }
    // else
    // {
        // get the image data
        $data = $_POST['image'];
        $companyName = $_POST['companyName'];

        list($type, $data) = explode(';', $data);
        list(, $data) = explode(',', $data);
        $data = base64_decode($data);

        //Image name
        //  $filename ="image". md5(uniqid()) . '.png';
        $filename = 'images/templates/'.$_POST['companyName'].'.png';

        // die($filename);

        $file = $filename;

        // decode the image data and save it to file
        file_put_contents($file, $data);

        $txtFilePath = 'images/templates/companyNames.txt';//companyNames.txt

        //$companyFile = fopen("companyNames.txt", "w")
        // file handling
        $companyFile = NULL;
        if (file_exists($txtFilePath)) 
        {
            $companyFile = fopen($txtFilePath, "a") or die("Could not open file");
        }
        else
        {
            $companyFile = fopen($txtFilePath, "w") or die("Could not create file");
        }
        $companyName = $_POST['companyName'] . "\n";
        fwrite($companyFile, $companyName);
        fclose($companyFile);
    // }
}
else
	die("No message found");
?>