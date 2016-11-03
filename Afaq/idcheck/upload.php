<?php
if (isset($_POST["image"]) && !empty($_POST["image"])) {

    // get the image data
    $data = $_POST['image'];

    list($type, $data) = explode(';', $data);
    list(, $data) = explode(',', $data);
    $data = base64_decode($data);

    //Image name
    //  $filename ="image". md5(uniqid()) . '.png';
    $filename = "image.png";

    $file = $filename;

    // decode the image data and save it to file
    file_put_contents($file, $data);
}
?>
