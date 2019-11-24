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
            $db_connection = pg_connect("host=localhost dbname=Library user=postgres password=library2")
            or die('Could not connect: ' . pg_last_error());
            
            $query = 'SELECT name FROM username';
            $usercomp = pg_query($query) or die('Query failed: ' . pg_last_error());

            // output
            while ($line = pg_fetch_array($usercomp, null, PGSQL_ASSOC))  {
                //echo  "\t<tr>\n";
                foreach ($line as $col_value) {
                    echo "\t\t<td>$col_value</td>\n"; // replace with current data
                    //echo "Success!\n";
                }
                //echo "\t</tr>\n";
            }
            // close connection
                echo "hello";
            pg_close($db_connection);
            ?></li>
                <li><button type="button" onclick="alert('test')"> Download </button></li>
            <hr><br>
            <li>3rd floor</li>
            <li><?php
            $db_connection = pg_connect("host=localhost dbname=Library user=postgres password=library2")
            or die('Could not connect: ' . pg_last_error());
            
            $query = 'SELECT name FROM username';
            $usercomp = pg_query($query) or die('Query failed: ' . pg_last_error());

            // output
            while ($line = pg_fetch_array($usercomp, null, PGSQL_ASSOC))  {
                //echo  "\t<tr>\n";
                foreach ($line as $col_value) {
                    echo "\t\t<td>$col_value</td>\n"; // replace with current data
                    //echo "Success!\n";
                }
                //echo "\t</tr>\n";
            }
            // close connection
                echo "hello";
            pg_close($db_connection);
            ?></li>
                <li><button type="button" onclick="alert('test2')"> Download </button></li>
            <hr><br>
            <li>4th floor</li>
            <li><?php
            $db_connection = pg_connect("host=localhost dbname=Library user=postgres password=library2")
            or die('Could not connect: ' . pg_last_error());
            
            $query = 'SELECT name FROM username';
            $usercomp = pg_query($query) or die('Query failed: ' . pg_last_error());

            // output
            while ($line = pg_fetch_array($usercomp, null, PGSQL_ASSOC))  {
                //echo  "\t<tr>\n";
                foreach ($line as $col_value) {
                    echo "\t\t<td>$col_value</td>\n"; // replace with current data
                    //echo "Success!\n";
                }
                //echo "\t</tr>\n";
            }
            // close connection
                echo "hello";
            pg_close($db_connection);
            ?></li>
                <li><button type="button" onclick="alert('test3')"> Download </button></li>
            <hr><br>
            <a href="login.php">Logout</a>
        </ul>
        </div>
    </body>
</html>
