# Primal Loader *1.2*

-----------------------------------------------------
Hello. This project was created in order to fix bad segments or "wrap a damaged DLL".
--

More precisely: Let's say you have a segment, relocations, and IAT. The segment is damaged, and it is also a dump because the relocations are already initialized by the system loader, in this case, primal-loader is an ideal solution.

Insert the segment (hex-view) into Segment.cpp, and IAT and relocations into SegmentHeaderManager.cpp. You can do the rest in your own framework.

-----------------------------------------------------

 # TODO.
 
  - Add gui.

------------------------------------------------------

# Credits
  * 0x000cb (me) - Project creator.
  * HoShiMin (www.github.com/HoShiMin) - Native help.