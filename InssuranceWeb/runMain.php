<?php


switch($_POST["callmain"])
{ 

case 'callmain':
ini_set('max_execution_time', 300); 
exec("./PassportOCR",$output);
foreach($output as $line)
	echo $line;
		break;    

default		
	break; 	
		
}   




	?>

