<html>

<body>

<form method="POST" action="/goform/ReadWrite">
  <input type=hidden name=redirect size=50 value="DAU_Webpage_uni_files/drill_config.asp">
  <b>Read / Write Paramteters</b>
  <p>Parameter Select <br>

   <select name="variable" size="1">
   
      <option value="<% WebPrint("var"); %>">        </option>
      <option value="WebPara_Gear">Motor Gear</option>
      <option value="Web_DisableWeightFilter">Disable Weight Filter</option>
      <option value="Web_DisableROPFilter">Disable ROP Filter</option>
      <option value="Web_WeightFilterBase">Weight Filter Base</option>
      <option value="Web_ROPFilterBase">ROP Filter Base</option>
     
   </select>

 <p><input type="submit" value="Read Parameter" name="read"></p>

  <p>Name<br>
  <input type="text" name="vari" size=60 maxsize=60 value="<% WebPrint("var"); %>" readonly></p>

   <p>Value<br>
  <input type="text" name="value" size=60 maxsize=60 value="<% WebPrint("val"); %>"></p>
  
  <p><input type="submit" value="Set Parameter" name="write"></p>
</form>

</body>


</html>