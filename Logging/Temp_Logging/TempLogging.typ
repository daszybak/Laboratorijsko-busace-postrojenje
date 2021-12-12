(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: TempLogging
 * File: TempLogging.typ
 * Author: fasch_f
 * Created: March 06, 2014
 ********************************************************************
 * Local data types of program TempLogging
 ********************************************************************)

TYPE
	LOGGING_STATES : 
		(
		WAIT_FOR_START := 0,
		CREATE_FILE := 1,
		WAIT_FOR_WRITE := 2,
		OPEN_FILE := 5,
		WRITE_FILE := 3,
		CLOSE_FILE := 4,
		ERROR := 200
		);
END_TYPE
