<!DOCTYPE html>
<html>
    <head>
    <title>Project Penguin</title>
        <link rel="stylesheet" type="text/css" href="global.css">
        <link rel="shortcut icon" type="image/x-icon" href="penguin.png" />
    </head>
    <body>
        <div class="form-container">
            <div class="user-img"></div>
        <ul class="list">
            <li>Lackluster Library Locator</li>
            <li>2nd floor</li>
            <li><?php
            $db_connection = pg_connect("host=10.34.44.133 dbname=testdb user=postgres password=library2") // host=localhost
            or die('Could not connect: ' . pg_last_error());
            
			$query = 'SELECT SUM(movein) FROM locationtimeinfo, locationid WHERE locationtimeinfo.id = locationid.id AND locationid.id = 1';
            $usercomp = pg_query($query) or die('Query failed: ' . pg_last_error());
			$query1 ='SELECT SUM(moveout) FROM locationtimeinfo, locationid WHERE locationtimeinfo.id = locationid.id AND locationid.id = 1';
			$usercomp1 = pg_query($query1) or die('Query failed: ' . pg_last_error());

            // output
			echo "Current people coming in: ";
            while ($line = pg_fetch_array($usercomp, null, PGSQL_ASSOC))  {
                foreach ($line as $col_value) {
                    echo "\t\t<td>$col_value</td>\n";
                }
                echo "<br><br>";
            }
			echo "Current people coming out: ";
			while ($line = pg_fetch_array($usercomp1, null, PGSQL_ASSOC))  {
				foreach ($line as $col_value) {
					echo "\t\t<td>$col_value</td>\n";
				}
			}
            // close connection
            pg_close($db_connection);
            ?></li>
<li><form name="form1" method="post" action="csv.php">
<label class="logoutButton">
<input name="submit2" type="submit" id="submit2" value="Download">
</label>
</form></li>
            <hr><br>
            <li>3rd floor</li>
<li><?php
	$db_connection = pg_connect("host=10.34.44.133 dbname=testdb user=postgres password=library2")
	or die('Could not connect: ' . pg_last_error());
	
	$query = 'SELECT SUM(movein) FROM locationtimeinfo, locationid WHERE locationtimeinfo.id = locationid.id AND locationid.id = 2';
	$usercomp = pg_query($query) or die('Query failed: ' . pg_last_error());
	$query1 ='SELECT SUM(moveout) FROM locationtimeinfo, locationid WHERE locationtimeinfo.id = locationid.id AND locationid.id = 2';
	$usercomp1 = pg_query($query1) or die('Query failed: ' . pg_last_error());
	
	// output
	echo "Current people coming in: ";
	while ($line = pg_fetch_array($usercomp, null, PGSQL_ASSOC))  {
		foreach ($line as $col_value) {
			echo "\t\t<td>$col_value</td>\n";
		}
		echo "<br><br>";
	}
	echo "Current people coming out: ";
	while ($line = pg_fetch_array($usercomp1, null, PGSQL_ASSOC))  {
		foreach ($line as $col_value) {
			echo "\t\t<td>$col_value</td>\n";
		}
	}
	// close connection
	pg_close($db_connection);
	?></li>
<li><li><form name="form1" method="post" action="csv.php">
<label class="logoutButton">
<input name="submit2" type="submit" id="submit2" value="Download">
</label>
</form></li></li>
            <hr><br>
            <li>4th floor</li>
<li><?php
	$db_connection = pg_connect("host=10.34.44.133 dbname=testdb user=postgres password=library2")
	or die('Could not connect: ' . pg_last_error());
	
	$query = 'SELECT SUM(movein) FROM locationtimeinfo, locationid WHERE locationtimeinfo.id = locationid.id AND locationid.id = 3';
	$usercomp = pg_query($query) or die('Query failed: ' . pg_last_error());
	$query1 ='SELECT SUM(moveout) FROM locationtimeinfo, locationid WHERE locationtimeinfo.id = locationid.id AND locationid.id = 3';
	$usercomp1 = pg_query($query1) or die('Query failed: ' . pg_last_error());
	
	// output
	echo "Current people coming in: ";
	while ($line = pg_fetch_array($usercomp, null, PGSQL_ASSOC))  {
		foreach ($line as $col_value) {
			echo "\t\t<td>$col_value</td>\n";
		}
		echo "<br><br>";
	}
	echo "Current people coming out: ";
	while ($line = pg_fetch_array($usercomp1, null, PGSQL_ASSOC))  {
		foreach ($line as $col_value) {
			echo "\t\t<td>$col_value</td>\n";
		}
	}
	// close connection
	pg_close($db_connection);
	?></li>
<li><li><form name="form1" method="post" action="csv.php">
<label class="logoutButton">
<input name="submit2" type="submit" id="submit2" value="Download">
</label>
</form></li></li>
            <hr><br>
<form name="form1" method="post" action="login.php">
<label class="logoutButton">
<input name="submit2" type="submit" id="submit2" value="Log out">
</label>
</form>
        </ul>
        </div>
    </body>
</html>
