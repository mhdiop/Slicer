#include "vtkObjectFactory.h"
#include "vtkCommand.h"
#include "vtkImageData.h"

#include "vtkSlicerSliceControllerWidget.h"
#include "vtkSlicerApplication.h"

#include "vtkKWWidget.h"
#include "vtkKWScaleWithEntry.h"
#include "vtkKWEntryWithLabel.h"
#include "vtkKWScale.h"
#include "vtkKWRenderWidget.h"
#include "vtkKWMenu.h"
#include "vtkKWMenuButton.h"



//---------------------------------------------------------------------------
vtkStandardNewMacro ( vtkSlicerSliceControllerWidget );
vtkCxxRevisionMacro ( vtkSlicerSliceControllerWidget, "$Revision: 1.0 $");


//---------------------------------------------------------------------------
vtkSlicerSliceControllerWidget::vtkSlicerSliceControllerWidget ( ) {

  //---  
  // widgets comprising the SliceControllerWidget for now.
  this->OffsetScale = NULL;
  this->OrientationMenu = NULL;
  this->ForegroundSelector = NULL;
  this->BackgroundSelector = NULL;
  this->LabelSelector = NULL;

  this->SliceNode = NULL;
  this->SliceCompositeNode = NULL;
}


//---------------------------------------------------------------------------
vtkSlicerSliceControllerWidget::~vtkSlicerSliceControllerWidget ( ){

    if ( this->OffsetScale ) {
        this->OffsetScale->Delete ( );
        this->OffsetScale = NULL;
    }
    if ( this->OrientationMenu ) {
        this->OrientationMenu->Delete ( );
        this->OrientationMenu = NULL;
    }
    if ( this->ForegroundSelector ) {
        this->ForegroundSelector->Delete ( );
        this->ForegroundSelector = NULL;
    }
    if ( this->BackgroundSelector ) {
        this->BackgroundSelector->Delete ( );
        this->BackgroundSelector = NULL;
    }
    if ( this->LabelSelector ) {
        this->LabelSelector->Delete ( );
        this->LabelSelector = NULL;
    }

    this->SetSliceNode ( NULL );
    this->SetSliceCompositeNode ( NULL );
}


//---------------------------------------------------------------------------
void vtkSlicerSliceControllerWidget::RemoveGUIObservers ( ) {

    this->OrientationMenu->GetWidget()->GetWidget()->GetMenu()->RemoveObservers ( vtkKWMenu::MenuItemInvokedEvent, this->GUICallbackCommand);
    this->ForegroundSelector->GetWidget()->GetWidget()->GetMenu()->RemoveObservers ( vtkKWMenu::MenuItemInvokedEvent, this->GUICallbackCommand);
    this->BackgroundSelector->GetWidget()->GetWidget()->GetMenu()->RemoveObservers ( vtkKWMenu::MenuItemInvokedEvent, this->GUICallbackCommand);
        this->LabelSelector->GetWidget()->GetWidget()->GetMenu()->RemoveObservers ( vtkKWMenu::MenuItemInvokedEvent, this->GUICallbackCommand);
    this->OffsetScale->GetWidget()->RemoveObservers ( vtkKWScale::ScaleValueChangingEvent, this->GUICallbackCommand );
    this->OffsetScale->GetWidget()->RemoveObservers ( vtkKWScale::ScaleValueChangedEvent, this->GUICallbackCommand );
    this->OffsetScale->GetWidget()->RemoveObservers ( vtkKWScale::ScaleValueStartChangingEvent, this->GUICallbackCommand );
        
}





//---------------------------------------------------------------------------
void vtkSlicerSliceControllerWidget::CreateWidget ( ) 
{

    if ( !this->MRMLScene ) {
        vtkErrorMacro ( << " MRML Scene must be set before creating widgets.");
        return;
    }

    // the widget is a frame with some widgets inside
    if (this->IsCreated ( ) ) {
        vtkErrorMacro ( << this->GetClassName() << "already created.");
        return;
    }
    this->Superclass::CreateWidget ( );
    
    //
    // Orientation  (TODO: make this into a vtkSlicerOrientationWidget)
    //
    this->OrientationMenu = vtkKWMenuButtonWithSpinButtonsWithLabel::New ();
    this->OrientationMenu->SetParent ( this );
    this->OrientationMenu->SetLabelWidth(3);
    this->OrientationMenu->Create ( );
    this->OrientationMenu->SetLabelText ( "Or: ");
    this->OrientationMenu->GetWidget()->GetWidget()->GetMenu()->AddObserver ( vtkKWMenu::MenuItemInvokedEvent, this->GUICallbackCommand);
    vtkKWMenuButton *mb = this->OrientationMenu->GetWidget()->GetWidget();
    mb->SetWidth ( 8 );
    mb->GetMenu()->AddRadioButton ( "Axial" );
    mb->GetMenu()->AddRadioButton ( "Sagittal" );
    mb->GetMenu()->AddRadioButton ( "Coronal" );
    mb->SetValue ("Axial");    

    //
    // Foreground, Background, and Label selections
    //
    this->ForegroundSelector = vtkSlicerNodeSelectorWidget::New();
    this->ForegroundSelector->SetParent ( this );
    this->ForegroundSelector->Create ( );
    this->ForegroundSelector->SetLabelText ("Fg:");
    this->ForegroundSelector->SetLabelWidth(3);
    this->ForegroundSelector->SetNodeClass ("vtkMRMLVolumeNode");
    this->ForegroundSelector->SetMRMLScene( this->MRMLScene );
    this->ForegroundSelector->GetWidget()->GetWidget()->SetMaximumLabelWidth(10);
    this->ForegroundSelector->GetWidget()->GetWidget()->GetMenu()->AddObserver ( vtkKWMenu::MenuItemInvokedEvent, this->GUICallbackCommand);
    this->BackgroundSelector = vtkSlicerNodeSelectorWidget::New();
    this->BackgroundSelector->SetParent ( this );
    this->BackgroundSelector->Create ( );
    this->BackgroundSelector->SetLabelText ("Bg:");
    this->BackgroundSelector->SetLabelWidth(3);
    this->BackgroundSelector->SetNodeClass ("vtkMRMLVolumeNode");
    this->BackgroundSelector->SetMRMLScene( this->MRMLScene );
    this->BackgroundSelector->GetWidget()->GetWidget()->SetMaximumLabelWidth(10);
    this->BackgroundSelector->GetWidget()->GetWidget()->GetMenu()->AddObserver ( vtkKWMenu::MenuItemInvokedEvent, this->GUICallbackCommand);
    this->LabelSelector = vtkSlicerNodeSelectorWidget::New();
    this->LabelSelector->SetParent ( this );
    this->LabelSelector->Create ( );
    this->LabelSelector->SetLabelText ("Lb:");
    this->LabelSelector->SetLabelWidth(3);
    this->LabelSelector->SetNodeClass ("vtkMRMLVolumeNode");
    this->LabelSelector->SetMRMLScene( this->MRMLScene );
    this->LabelSelector->GetWidget()->GetWidget()->SetMaximumLabelWidth(10);
    this->LabelSelector->GetWidget()->GetWidget()->GetMenu()->AddObserver ( vtkKWMenu::MenuItemInvokedEvent, this->GUICallbackCommand);
        
    //
    // Create a scale to control the slice number displayed
    //
    this->OffsetScale = vtkKWScaleWithEntry::New();
    this->OffsetScale->SetParent ( this );
    this->OffsetScale->Create();
    this->OffsetScale->RangeVisibilityOff ( );
    this->OffsetScale->SetEntryWidth(8);
    this->OffsetScale->SetLabelPositionToLeft();
    this->OffsetScale->GetWidget()->AddObserver( vtkKWScale::ScaleValueChangingEvent, this->GUICallbackCommand );
    this->OffsetScale->GetWidget()->AddObserver( vtkKWScale::ScaleValueChangedEvent, this->GUICallbackCommand );
    this->OffsetScale->GetWidget()->AddObserver( vtkKWScale::ScaleValueStartChangingEvent, this->GUICallbackCommand );
            
    this->Script("grid %s %s -sticky ew", 
            this->OrientationMenu->GetWidgetName(), this->ForegroundSelector->GetWidgetName());
    this->Script("grid %s %s -sticky ew", 
            this->LabelSelector->GetWidgetName(), this->BackgroundSelector->GetWidgetName());
    this->Script("grid %s -sticky ew -columnspan 2", this->OffsetScale->GetWidgetName());

}

//----------------------------------------------------------------------------
void vtkSlicerSliceControllerWidget::ProcessWidgetEvents ( vtkObject *caller, unsigned long event, void *callData ) 
{ 
  if ( !this->SliceNode )
    {
    return;
    }

  if ( this->OffsetScale->GetWidget() == vtkKWScale::SafeDownCast( caller ) &&
          event == vtkKWScale::ScaleValueStartChangingEvent )
    {
    // set an undo state when the scale starts being dragged
    this->MRMLScene->SaveStateForUndo( this->SliceNode );
    }
  
  int modified = 0;

  // Set the Offset from the Scale
  // - transform the value by SliceToRAS so that the 
  //   slice's 'z' coordinate corresponds to the direction
  //   perpendicular to the current slice orientation
  // - basically, multiply the scale value times the z column vector of the
  //   rotation matrix
  vtkMatrix4x4 *newm = vtkMatrix4x4::New();
  newm->DeepCopy( this->SliceNode->GetSliceToRAS() );
  double in[4], out[4];
  in[0] = in[1] = in[3] = 0.;
  in[2] = (double) this->OffsetScale->GetValue();
  newm->MultiplyPoint(in, out);
  newm->SetElement( 0, 3, out[0] );
  newm->SetElement( 1, 3, out[1] );
  newm->SetElement( 2, 3, out[2] );
 
  if ( newm->GetElement( 0, 3 ) != this->SliceNode->GetSliceToRAS()->GetElement( 0, 3 ) ||
       newm->GetElement( 1, 3 ) != this->SliceNode->GetSliceToRAS()->GetElement( 1, 3 ) ||
       newm->GetElement( 2, 3 ) != this->SliceNode->GetSliceToRAS()->GetElement( 2, 3 ) )
    {
    this->SliceNode->GetSliceToRAS()->DeepCopy( newm );
    this->SliceNode->UpdateMatrices();
    modified = 1;
    }

  if ( vtkKWMenu::SafeDownCast(caller) == this->OrientationMenu->GetWidget()->GetWidget()->GetMenu() )
    {
    vtkKWMenuButton *mb = this->OrientationMenu->GetWidget()->GetWidget();
    if ( !strcmp (mb->GetValue(), "Axial") )   
      {
      this->SliceNode->SetOrientationToAxial();
      }
    if ( !strcmp (mb->GetValue(), "Sagittal") )   
      {
      this->SliceNode->SetOrientationToSagittal();
      }
    if ( !strcmp (mb->GetValue(), "Coronal") )   
      {
      this->SliceNode->SetOrientationToCoronal();
      }
    }

  if ( vtkKWMenu::SafeDownCast(caller) == this->ForegroundSelector->GetWidget()->GetWidget()->GetMenu() )
    {
    this->SliceCompositeNode->SetForegroundVolumeID( 
            this->ForegroundSelector->GetSelected()->GetID() );
    }
  if ( vtkKWMenu::SafeDownCast(caller) == this->BackgroundSelector->GetWidget()->GetWidget()->GetMenu() )
    {
    this->SliceCompositeNode->SetBackgroundVolumeID( 
            this->BackgroundSelector->GetSelected()->GetID() );
    }
  if ( vtkKWMenu::SafeDownCast(caller) == this->LabelSelector->GetWidget()->GetWidget()->GetMenu() )
    {
        // Nothing yet - TODO: the slice logic needs to handle labelmaps
    }

  if ( modified )
    {
    this->Modified();
    }

}

//----------------------------------------------------------------------------
void vtkSlicerSliceControllerWidget::ProcessMRMLEvents ( vtkObject *caller, unsigned long event, void *callData ) 
{ 


  if ( !this->SliceNode )
    {
    return;
    }
  
  int modified = 0;


  // 
  // Update the menu to match the node
  //
  vtkKWMenuButton *mb = this->OrientationMenu->GetWidget()->GetWidget();
  mb->SetValue( this->SliceNode->GetOrientationString() );

  //
  // Set the scale range to match the field of view
  //
  double fovover2 = this->SliceNode->GetFieldOfView()[2] / 2.;
  double min, max;
  this->OffsetScale->GetRange(min, max);
  if ( min != -fovover2 || max != fovover2 )
    {
    this->OffsetScale->SetRange(-fovover2, fovover2);
    modified = 1;
    }


  //
  // Set the scale value to match the offset
  //
  // Since translation is a scalar multiple of the Pz column of the
  // SliceToRAS upper 3x3, find a non-zero entry in that column
  // and calculate the scalar from that.
  vtkMatrix4x4 *m = this->SliceNode->GetSliceToRAS();
  int i;
  double s;
  for (i = 0; i < 3; i++)
    {
    if ( m->GetElement( i, 2 ) != 0.0 )
      { 
      s = m->GetElement( i, 3 ) / m->GetElement( i, 2 ); 
      }
    }

  if ( s != this->OffsetScale->GetValue() )
    {
    this->OffsetScale->SetValue( s );
    modified = 1;
    }

  //
  // when the composite node changes, update the menus to match
  //
  if ( caller == this->SliceCompositeNode )
    {
    vtkMRMLNode *node = this->MRMLScene->GetNodeByID( this->SliceCompositeNode->GetForegroundVolumeID() );
    if ( node )
      {
      this->ForegroundSelector->SetSelected(node);
      }
    }

  if ( caller == this->SliceCompositeNode )
    {
    vtkMRMLNode *node = this->MRMLScene->GetNodeByID( this->SliceCompositeNode->GetBackgroundVolumeID() );
    if ( node )
      {
      this->BackgroundSelector->SetSelected(node);
      }
    }

  //
  //  Trigger events if needed
  //
  if ( modified )
    {
    this->Modified();
    }
}

//----------------------------------------------------------------------------
void vtkSlicerSliceControllerWidget::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  // widgets?
}

