<?php
// connect to database
$db_connection = pg_connect("host=10.34.44.133 dbname=testdb user=postgres password=library2")
or die('Could not connect: ' . pg_last_error());
	
$query = 'SELECT SUM(movein) FROM locationtimeinfo, locationid WHERE locationtimeinfo.id = locationid.id AND locationid.id = 2';
$usercomp = pg_query($query) or die('Query failed: ' . pg_last_error());

// output headers so that the file is downloaded rather than displayed
header('Content-type: text/csv');
header('Content-Disposition: attachment; filename="demo.csv"');
 
// do not cache the file
header('Pragma: no-cache');
header('Expires: 0');
 
// create a file pointer connected to the output stream
$file = fopen('php://output', 'w');
 
// send the column headers
fputcsv($file, array('Column 1', 'Column 2', 'Column 3'));

while ($line = pg_fetch_array($usercomp, null, PGSQL_ASSOC))  {
	foreach ($line as $row) {
 		fputcsv($file, $row); // need to test this out with different floors
	}
}
	
exit();
?>
