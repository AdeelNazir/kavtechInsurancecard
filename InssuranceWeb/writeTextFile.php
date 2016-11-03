<?php
if (isset($_GET["text"]) && !empty($_GET["text"])) 
{
    // get the image data
    $txt = "\n".$_GET['text'];
    $mode = $_GET['mode'];
    $txtFilePath = NULL;

    if ($mode == 1) {
        $txtFilePath = 'ocrTxtFiles/group.txt';//companyNames.txt
    }
    if ($mode == 2) {
        $txtFilePath = 'ocrTxtFiles/id.txt';//companyNames.txt
    }
    if ($mode == 3) {
        $txtFilePath = 'ocrTxtFiles/name.txt';//companyNames.txt
    }

    if ($txtFilePath != NULL) 
    {
        $dynamicFile = fopen($txtFilePath, "a") or die("Could not open file");
        fwrite($dynamicFile, $txt);
        fclose($dynamicFile);
    }
    else
        die("path is empty");
}
else
	die("No message found");
?>