<?xml version="1.0" encoding="UTF-8"?> 
<java version="1.6.0_26" class="java.beans.XMLDecoder"> 
 <object class="com.ti.dvt.datamodel.core.DataModel"> 
  <void property="filePath"> 
   <string>C:\Arquivos\Doutorado\Sincrotron\Implementacao\.metadata\.plugins\com.ti.dvt.graph.visualization\savedSolutions\1</string> 
  </void> 
  <void method="addSource"> 
   <object class="com.ti.dvt.datamodel.ccsgraphs.CCSGraphsJtagDataSource"> 
    <void method="connectOutput"> 
     <object class="com.ti.dvt.datamodel.ccsgraphs.SingleDualTimeCCSGraphDecoder"> 
      <void method="connectOutput"> 
       <object class="com.ti.dvt.datamodel.ccsgraphs.GraphScaleDecoder"> 
        <void method="connectOutput"> 
         <object class="com.ti.dvt.datamodel.core.CircularBuffer"> 
          <void property="name"> 
           <string>CircularBuffer</string> 
          </void> 
          <void property="recordsBlockMode"> 
           <boolean>true</boolean> 
          </void> 
          <void property="recordsPerBlock"> 
           <int>200</int> 
          </void> 
          <void property="size"> 
           <int>200</int> 
          </void> 
         </object> 
        </void> 
        <void property="dspDataType"> 
         <string>32 bit floating point</string> 
        </void> 
        <void property="name"> 
         <string>GraphScaleDecoder</string> 
        </void> 
        <void property="samplingRate"> 
         <int>10</int> 
        </void> 
       </object> 
      </void> 
      <void property="blockSize1"> 
       <int>200</int> 
      </void> 
      <void property="blockSize2"> 
       <int>200</int> 
      </void> 
      <void property="dspDataType"> 
       <string>32 bit floating point</string> 
      </void> 
      <void property="name"> 
       <string>SingleDualTimeCCSGraphDecoder</string> 
      </void> 
     </object> 
    </void> 
    <void property="blockSize1"> 
     <int>200</int> 
    </void> 
    <void property="blockSize2"> 
     <int>200</int> 
    </void> 
    <void property="dspDataType"> 
     <string>32 bit floating point</string> 
    </void> 
    <void property="name"> 
     <string>CCSGraphsJtagDataSource</string> 
    </void> 
    <void property="startAddress1"> 
     <long>41024</long> 
    </void> 
    <void property="startAddress1Expr"> 
     <string>&amp;vect</string> 
    </void> 
   </object> 
  </void> 
 </object> 
</java> 
