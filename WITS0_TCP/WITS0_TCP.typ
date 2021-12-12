(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: WITS0_TCP
 * File: WITS0_TCP.typ
 * Author: fasch_f
 * Created: May 04, 2012
 ********************************************************************
 * Local data types of program WITS0_TCP
 ********************************************************************)
(*states of TCP server and client task*)

TYPE
	TCP_STATES : 
		(
		SEL_IF := 1, (*select used etherenet interface*)
		TCP_OPEN := 2, (*open a tcp socket*)
		TCP_SERVER := 3, (*start a tcp server*)
		TCP_CLIENT := 4, (*start a tcp client*)
		TCP_CLOSE := 5,
		TCP_WAIT := 200, (*wait for commands*)
		TCP_FAIL := 250 (*fail at an operatioon on tcp interface*)
		);
END_TYPE

(*status of a tcp connection (data to send...)*)

TYPE
	TCP_INFO_STATES : 
		(
		DATA_SENT := 0, (*data succesfully sent*)
		DATA_TO_SEND := 1, (*data has to be send to deticated client*)
		DATA_SEND_FAIL := 2, (*fail at sending data*)
		CLIENT_CONNECTED := 3, (*Tcp Client FUB succesfull*)
		CLIENT_UNCONNECTED := 4 (*Tcp Client FUB failed*)
		);
END_TYPE

(*information structure for tcp communication*)

TYPE
	TCP_INFO_STRUCT : 	STRUCT 
		sIP : STRING[16]; (*IP Address of the TCP client*)
		udiIdent : UDINT; (*TCP client ident*)
		udiPort : UDINT; (*TCP client port*)
		enTCPStatus : TCP_INFO_STATES; (*status of the server/client connection*)
	END_STRUCT;
END_TYPE
