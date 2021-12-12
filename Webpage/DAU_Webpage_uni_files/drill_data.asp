<html>
<head>
<META HTTP-EQUIV="REFRESH" CONTENT="1" URL='drill_data.asp'> <!-- autorefresh -->
</head>

<body>

  
<table border="1" frame="box">
	<tr>
		<th><i><big>Information</big></i></th>
		<th><i><big>Value</big></i></th>
		<th><i><big>Unit</big></i></th>
		<th><i><big></big></th>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>Block Postion</b></td>
		<td><b><%ReadPLC ("Web_Blockpos"); %></b></td>
		<td><b>[mm]</b></td>
		<td><br>
		<form method="POST" action="/goform/ReadWrite">
  			<input type=hidden name=redirect size=50 value="DAU_Webpage_uni_files/drill_data.asp">
			<input type="hidden" name="variable" value="Web_PosInit">
			<input type="hidden" name="value" value="1">
			<input type="submit" value="Reset Block Position" name="write">
		</form>
		</td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>Rate of Penetration</b></td>
		<td><b><%ReadPLC ("Web_ROP"); %></b></td>
		<td><b>[mm/s]</b></td>
		<td> </td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>ROP Filter disabled</b></td>
		<td><b><%ReadPLC ("Web_DisableROPFilter"); %></b></td>
		<td><b>[]</b></td>
		<td>0...filtering on / 1...filtering off</td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>ROP Filter Base</b></td>
		<td><b><%ReadPLC ("Web_ROPFilterBase"); %></b></td>
		<td><b>[100 ms]</b></td>
		<td>moving average filter, e.g. 10 = 10*100ms = 1 s</td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>actual Hookload</b></td>
		<td><b><%ReadPLC ("Web_Hookload"); %></b></td>
		<td><b>[kg]</b></td>
		<td></td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>Hookload Filter disabled</b></td>
		<td><b><%ReadPLC ("Web_DisableWeightFilter"); %></b></td>
		<td><b>[]</b></td>
		<td>0...filtering on / 1...filtering off</td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>Hookload Filter Base</b></td>
		<td><b><%ReadPLC ("Web_WeightFilterBase"); %></b></td>
		<td><b>[100 ms]</b></td>
		<td>moving average filter, e.g. 10 = 10*100ms = 1 s</td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>max. Hookload</b></td>
		<td><b><%ReadPLC ("Web_MaxHookload"); %></b></td>
		<td><b>[kg]</b></td>
		<td><br>
		<form method="POST" action="/goform/ReadWrite">
  			<input type=hidden name=redirect size=50 value="DAU_Webpage_uni_files/drill_data.asp">
			<input type="hidden" name="variable" value="Web_ResetMaxHook">
			<input type="hidden" name="value" value="1">
			<input type="submit" value="Reset max. Hookload" name="write">
		</form>
		</td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>act. Weight on Bit</b></td>
		<td><b><%ReadPLC ("Web_WOB"); %></b></td>
		<td><b>[kg]</b></td>
		<td></td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>Torque</b></td>
		<td><b><%ReadPLC ("Web_Torque"); %></b></td>
		<td><b>[Nm]</b></td>
		<td></td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>Rotary Speed</b></td>
		<td><b><%ReadPLC ("Web_RPM"); %></b></td>
		<td><b>[rpm]</b></td>
		<td></td>
	</tr>
	<tr>
		<td></td>
		<td></td>
		<td></td>
		<td></td>
	</tr>
	<tr>
		<td><b>Distance to probe</b></td>
		<td><b><%ReadPLC ("Web_Sonic"); %></b></td>
		<td><b>[mm]</b></td>
		<td></td>
	</tr>

</table>

</body>

</html>