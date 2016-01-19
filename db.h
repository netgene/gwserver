#ifndef GW_DB_H_
#define GW_DB_H_

#include <iostream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

namespace gwsudoku {

class Db
{
 public:
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    Db()
    {
    	/* Create a connection */
    	driver = get_driver_instance();
    	con = driver->connect("tcp://127.0.0.1:3306", "root", "0709");
    	/* Connect to the MySQL test database */
    	con->setSchema("shudang");
    	stmt = con->createStatement();
    }
    
    void dbQuery()
    {
    	res = stmt->executeQuery("SELECT name from sdbook limit 1");
    	while (res->next()) {
    	  std::cout << "\t... MySQL replies: ";
    	  /* Access column data by alias or column name */
    	  std::cout << res->getString("name") << std::endl;
    	  std::cout << "\t... MySQL says it again: ";
    	  /* Access column fata by numeric offset, 1 is the first column */
    	  std::cout << res->getString(1) << std::endl;
    	}
    }

    ~Db()
    {
    	delete res;
    	delete stmt;
    	delete con;
    }
};

// Implementation


} // end gwsudoku

#endif // GW_DB_H_
