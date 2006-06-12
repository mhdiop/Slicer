
#include "vtkObjectFactory.h"
#include "vtkSlicerModuleNavigationIcons.h"

//---------------------------------------------------------------------------
vtkStandardNewMacro ( vtkSlicerModuleNavigationIcons );
vtkCxxRevisionMacro ( vtkSlicerModuleNavigationIcons, "$Revision: 1.0 $");

//---------------------------------------------------------------------------
vtkSlicerModuleNavigationIcons::vtkSlicerModuleNavigationIcons ( )
{
    //--- Module navigation frame icons
    this->ModulePrevIcon = vtkKWIcon::New ( );
    this->ModuleNextIcon = vtkKWIcon::New ( );
    this->ModuleHistoryIcon = vtkKWIcon::New ( );
    this->AssignImageDataToIcons ( );
}

//---------------------------------------------------------------------------
vtkSlicerModuleNavigationIcons::~vtkSlicerModuleNavigationIcons ( )
{
    // module navigation images
    if ( this->ModuleNextIcon ) {
        this->ModuleNextIcon->Delete ( );
        this->ModuleNextIcon = NULL;
    }
    if ( this->ModulePrevIcon ) {
        this->ModulePrevIcon->Delete ( );
        this->ModulePrevIcon = NULL;
    }
    if ( this->ModuleHistoryIcon ) {
        this->ModuleHistoryIcon->Delete ( );
        this->ModuleHistoryIcon = NULL;
    }
    
}



//---------------------------------------------------------------------------
void vtkSlicerModuleNavigationIcons::AssignImageDataToIcons ( )
{

    this->ModuleHistoryIcon->SetImage( image_ModuleHistory,
                                    image_ModuleHistory_width,
                                    image_ModuleHistory_height,
                                    image_ModuleHistory_pixel_size, 0, 0);
    
    this->ModulePrevIcon->SetImage( image_ModulePrev,
                                    image_ModulePrev_width,
                                    image_ModulePrev_height,
                                    image_ModulePrev_pixel_size, 0 , 0);
    
    this->ModuleNextIcon->SetImage( image_ModuleNext,
                                       image_ModuleNext_width,
                                       image_ModuleNext_height,
                                       image_ModuleNext_pixel_size, 0, 0);
}



//---------------------------------------------------------------------------
void vtkSlicerModuleNavigationIcons::PrintSelf ( ostream& os, vtkIndent indent )
{
    this->vtkObject::PrintSelf ( os, indent );

    os << indent << "SlicerModuleNavigationIcons: " << this->GetClassName ( ) << "\n";

   os << indent << "ModuleNext" << this->GetModuleNextIcon () << "\n";
   os << indent << "ModulePrev" << this->GetModulePrevIcon () << "\n";
   os << indent << "ModuleHistory" << this->GetModuleHistoryIcon () << "\n";
   
}

