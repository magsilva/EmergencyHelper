//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>

//---------------------------------------------------------------------------


class TEmergencyHelper : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog;
        TMainMenu *MainMenu;
        TMenuItem *MenuArquivo;
        TMenuItem *MenuArquivoAbrir;
        TMenuItem *MenuArquivoSair;
	TMenuItem *MenuComando;
        TMenuItem *MenuComandoGerarAcidente;
        TImage *ImagemGrafo;
        TMenuItem *MenuAjuda;
        TMenuItem *MenuArquivoFechar;
        TMenuItem *MenuComandoAcharMelhorRota;
        TMenuItem *MenuAjudaSobre;
        TToolBar *ToolBar1;
	TToolButton *ToolButtonAbrir;
	TToolButton *ToolButtonFechar;
        TToolButton *ToolButton3;
	TToolButton *ToolButtonGerarAcidente;
	TToolButton *ToolButtonAcharMelhorRota;
        TToolButton *ToolButton6;
	TToolButton *ToolButtonAjuda;
        TImageList *ImageList;
	TMenuItem *MenuComandoDistribuirAmbulancias;
	TToolButton *ToolButtonDistribuirAmbulancias;
	TToolButton *ToolButtonConstruirHospitais;
	TMenuItem *MenuComandoConstruirHospitais;
        void __fastcall MenuArquivoAbrirClick(TObject *Sender);
        void __fastcall MenuArquivoSairClick(TObject *Sender);
        void __fastcall MenuComandoGerarAcidenteClick(TObject *Sender);
        void __fastcall ToolButtonAbrirClick(TObject *Sender);
        void __fastcall MenuArquivoFecharClick(TObject *Sender);
        void __fastcall ToolButtonFecharClick(TObject *Sender);
	void __fastcall MenuComandoDistribuirAmbulanciasClick(TObject *Sender);
	void __fastcall MenuComandoConstruirHospitaisClick(TObject *Sender);
	void __fastcall MenuComandoAcharMelhorRotaClick(TObject *Sender);
	void __fastcall ToolButtonDistribuirAmbulanciasClick(TObject *Sender);
	void __fastcall ToolButtonConstruirHospitaisClick(TObject *Sender);
	void __fastcall ToolButtonGerarAcidenteClick(TObject *Sender);
	void __fastcall ToolButtonAcharMelhorRotaClick(TObject *Sender);
	void __fastcall ToolButtonAjudaClick(TObject *Sender);
	void __fastcall MenuAjudaSobreClick(TObject *Sender);
private:
public:
        void AbrirGrafo();
        void GerarAcidente();
        void FecharGrafo();
        void DesenharAresta(TPoint origem, TPoint destino, TColor cor);
        void CalcularEscala();
        void DesenharSeta(TPoint origem, TPoint destino);
        void DesenharSetaGambiarra(float x1, float y1, float x11, float y11);
        void DesenharQuadrado(TColor cor, TPoint p, AnsiString texto);
	void DesenharCirculo(TColor cor, TPoint p, AnsiString texto);
        void DistribuirAmbulancias();
 	void ConstruirHospitais();
        void AcharMelhorRota();
        __fastcall TEmergencyHelper(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEmergencyHelper *EmergencyHelper;
//---------------------------------------------------------------------------
#endif
