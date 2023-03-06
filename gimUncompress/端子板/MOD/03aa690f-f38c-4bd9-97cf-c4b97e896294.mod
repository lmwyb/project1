<?xml version="1.0" encoding="utf-8"?>
<Device>
  <Entities>
	<Entity ID="28" Type="simple" Visible="true">
      <Cuboid L="5" W="7" H="8" />
      <Color R="138" G="149" B="151" A="0" />
      <TransformMatrix Value="1,0,0,0,0,1,0,0,0,0,1,0,5,5,5,1" />
    </Entity>
    <Entity ID="19" Type="simple" Visible="true">
      <TerminalBlock L="85" W="80" T="15" CL="5" CS="40" RS="40" R="7" CN="2" RN="2" BL="23" Phase="A" />
      <Color R="138" G="149" B="151" A="0" />
      <TransformMatrix Value="1,0,0,0,0,1,0,0,0,0,1,0,-5,-5,-5,1" />
    </Entity>
    <Entity ID="6378" Type="simple" Visible="false">
      <TruncatedCone BR="50" TR="35" H="20" />
      <Color R="255" G="255" B="255" A="0" />
      <TransformMatrix Value="1,0,0,0,0,1,0,0,0,0,1,0,0,7.105427357601E-15,-25,1" />
    </Entity>
    <Entity ID="6379" Type="simple" Visible="false">
      <OffsetRectangularTable TL="120" TW="25" LL="120" LW="30" H="10" XOFF="0" YOFF="0" />
      <Color R="255" G="255" B="255" A="0" />
      <TransformMatrix Value="0.707106781186548,-0.707106781186547,0,0,0.707106781186547,0.707106781186548,0,0,0,0,1,0,-0.0217536017297988,-0.0178567022084906,-20,1" />
    </Entity>
    <Entity ID="6380" Type="boolean" Visible="true">
      <Boolean Type="Difference" Entity1="6378" Entity2="6379" />
      <Color R="255" G="255" B="255" A="0" />
      <TransformMatrix Value="1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1" />
    </Entity>
  </Entities>
</Device>