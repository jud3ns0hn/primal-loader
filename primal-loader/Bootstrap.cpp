#include "hpp/Vendor.hpp"

/**
 *
 *                                             ------------------------------------------ o
 *                                           |              How it's works ?              |
 *                                           |   (Basically it's just modified mmap :D)   |
 *                                           o ------------------------------------------
 *                                                             | Credits |
 *                                                            |  ^^^^^^^  |
 *                                                       o -------------------- o
 *                                                      | 0x000cb  - project dev |
 *                                                      | HoShiMin - native help |
 *                                                       o -------------------- o
 *
 *
 *                               ~~~~~~~~~~~
 *                    +----> + < Relocations > + > > >
 * o---------o        |          ~~~~~~~~~~~                    o-----------------------o         o-----------------------o         o----------o
 * [ Segment  > > > > O                              > > > > - | Reconstruct at runtime  | > > > | Call internal framework | > > > | Invoke OEP |
 * o---------o        |          ~~~~~~~~~~~                    o-----------+-----------o         o-----------------------o         o-----+----o
 *                    +----> + <   Imports   > + > > >                      |->  >  >  >  >  >  >  >  >  >  >    <  <  <  <  <  <  <  <  <-|
 *                               ~~~~~~~~~~~ 
 *
 *
 */

BOOL APIENTRY DllMain (HMODULE module, DWORD callReason, LPVOID lpReserved) {
    
    if (callReason == DLL_PROCESS_ATTACH) {

          ///////////////////////////////////////////////////////////////////////////////////////
        //                                                                                      //
        // LOADER.                                                                              //
        //                                                                                      //
           Segment segment = Singleton <Segment> :: GetInstance ();                             //
           Logger  log     = Singleton <Logger>  :: GetInstance ();                             //
        //                                                                                      //
        //////////////////////////////////////////////////////////////////////////////////////////
        //                                                                                      //
           RuntimeHandler runtime (                                                             //
                                                                                                //
        //DON'T TOUCH ME                                                                        //
             segment,                                                                           //
        //NEW ALLOCATION ADDRESS. (In theory, you can leave it that way)                        //
             0x0,                                                                               //
        //OLD ALLOCATION ADDRESS. (Previosly runtime value for fix relocations)                 //
             0x0,                                                                               //
        //SEGMENT SIZE.    (Param for copy segment to alloc mem. Used for memcpy)               //
             0x0,                                                                               //
        //ALLOCATION SIZE. (Param for new memory alloc size. Used for VirtualAlloc)             //                           
             0x0,                                                                               //
        //OEP OFFSET.      (https://en.wikipedia.org/wiki/Entry_point)                          //
             0x0                                                                                //
                                                                                                //
           );                                                                                   //
        //                                                                                      //
        //////////////////////////////////////////////////////////////////////////////////////////
        //                                                                                      //
        // SEGMENT.                                                                             //
        //                                                                                      //
           SegmentInterpreter framework = Singleton <SegmentInterpreter> :: GetInstance ();     //
        //                                                                                      //
         ///////////////////////////////////////////////////////////////////////////////////////

        //Alloc memory && init variable.
        runtime.ExtractSegment ();

        log.Info (Logger::Type::INFO, std::string ("Memory allocated. Base at: 0x").append (std::to_string (Primal::GetAllocationPointer ())));

        //Fixup relocs && imports.
        runtime.ReconstructHotPoints ();

        log.Info (Logger::Type::INFO, "Relocations and imports reconstructed.");

        //Fixes, patches, and stuff.
        framework.CallbackWithOEP (SegmentTranslator::CallbackType::BEFORE);
        
        //Make segment alive.
        runtime.InvokeOEP ();

        log.Info (Logger::Type::INFO, "OEP invoked.");

        //Just stuff.
        framework.CallbackWithOEP (SegmentTranslator::CallbackType::AFTER);

    }
    
    return TRUE;

}