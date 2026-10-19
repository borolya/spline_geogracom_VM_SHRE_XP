//---------------------------------------------------------------------------

#ifndef UFManTracksH
#define UFManTracksH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "UnFManFiles.h"
//---------------------------------------------------------------------------
class TFManTracks : public TFManFiles
{
__published:	// IDE-managed Components
        void __fastcall ListBox1DblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFManTracks(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFManTracks *FManTracks;
extern AnsiString trackfilename;
//---------------------------------------------------------------------------
#endif
