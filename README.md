# To run the game without the server client
## Run this on your terminal
```
make
```
## Then run this
```
./main
```



# To run the db code
## To start a mysql server type this onto your terminal
```
mysql.server start
```

## To stop a mysql server type this onto your terminal
```
mysql.server stop
```

## To compile the database code
```
gcc -I/opt/homebrew/opt/mysql/include/mysql/ -o db database.c -L/opt/homebrew/opt/mysql/lib -lmysqlclient
```

## To run the database code
```
./db
```