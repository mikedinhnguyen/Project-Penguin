<?php
    $username = $_POST['username'];
    $password = $_POST['password'];
    
    $db_connection = pg_connect("host=localhost dbname=Library user=postgres password=library2")
    or die('Could not connect: ' . pg_last_error());
    
    $query = 'SELECT name FROM username';
    $usercomp = pg_query($query) or die('Query failed: ' . pg_last_error());
    $query2 = 'SELECT password FROM username';
    $passcomp = pg_query($query2) or die('Query failed: ' . pg_last_error());
    
    $user_bool = false;
    $pass_bool = false;

    // output
    while ($line = pg_fetch_array($usercomp, null, PGSQL_ASSOC))  {
        //echo  "\t<tr>\n";
        foreach ($line as $col_value) {
            if ($username == $col_value){
                //echo "\t\t<td>$col_value</td>\n";
                //echo "Success!\n";
                $user_bool = true;
            }
        }
        //echo "\t</tr>\n";
    }

    while ($line = pg_fetch_array($passcomp, null, PGSQL_ASSOC))  {
        //echo  "\t<tr>\n";
        foreach ($line as $col_value) {
            if ($password == $col_value){
                //echo "\t\t<td>$col_value</td>\n";
                //echo "Success!\n";
                $pass_bool = true;
            }
        }
        //echo "\t</tr>\n";
    }
    
    if ($user_bool == true && $pass_bool == true){
        header('Location: hub.html'); // hub.html or hub.php is fine
    }
    else{
        header('Location: login.php');
        $message = "The username and/or password was incorrect. Please try again.";
        echo "$message";
    }
    
    // free resultset
    // pg_free_result($result);
    
    // close connection
    pg_close($db_connection);
?>
