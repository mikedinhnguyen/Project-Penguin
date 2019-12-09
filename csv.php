<?php
// connect to database
$db_connection = pg_connect("host=24.7.172.204 dbname=testdb user=postgres password=library2")
or die('Could not connect: ' . pg_last_error());

$query = 'SELECT SUM(movein) FROM locationtimeinfo, locationid WHERE locationtimeinfo.id = locationid.id AND locationid.id = 1';
$usercomp = pg_query($query) or die('Query failed: ' . pg_last_error());
$query1 ='SELECT SUM(moveout) FROM locationtimeinfo, locationid WHERE locationtimeinfo.id = locationid.id AND locationid.id = 1';
$usercomp1 = pg_query($query1) or die('Query failed: ' . pg_last_error());

// output headers so that the file is downloaded rather than displayed
header('Content-type: text/csv');
header('Content-Disposition: attachment; filename="demo.csv"');

// do not cache the file
header('Pragma: no-cache');
header('Expires: 0');

// create a file pointer connected to the output stream
$file = fopen('php://output', 'w');

// send the column headers
fputcsv($file, array('Current people coming in', 'Current people coming out'));

/*
// output each row of the data
foreach ($data as $row)
{
fputcsv($file, $row);
}*/
	
$data = array(pg_fetch_array($usercomp, null, PGSQL_ASSOC), pg_fetch_array($usercomp1, null, PGSQL_ASSOC));
		foreach ($data as $col)
			{fputcsv($file, $col);}
	/*
while ($line = pg_fetch_array($usercomp, null, PGSQL_ASSOC))  {
	fputcsv($file, $line); // need to test this out with different floors
}
while ($line = pg_fetch_array($usercomp1, null, PGSQL_ASSOC))  {
	fputcsv($file, $line); // need to test this out with different floors
}*/
	fclose($file);
exit();
?>
