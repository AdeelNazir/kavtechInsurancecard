<?php
switch($_GET["process"])
{ 
case 'doProcess':
    echo "got in \n";
    echo "start ececution \n";
    ini_set('max_execution_time', 5000); 
    exec("./insurance_ocr",$output);
    // echo $output;
    echo "\n executed";
    foreach($output as $line)
        echo $line;
    echo "\n End";
            break;    
default:     
    break;  
        
}   
    ?>

