//---------------------------------------------------------------------------
#include <vcl.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <vcl/dstring.h>
#include <math.h>
#pragma hdrstop
#include "Grafo.cpp"
#include "Unit1.h"
#include "Unit2.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEmergencyHelper *EmergencyHelper;
float escala = 0;
int acidente = 0;
Grafo *Graph = NULL;


//---------------------------------------------------------------------------
__fastcall TEmergencyHelper::TEmergencyHelper(TComponent* Owner)
    : TForm(Owner)
{
	MenuComando->Enabled = false;
    MenuArquivoFechar->Enabled = false;
	ToolButtonFechar->Enabled = false;
}

//---------------------------------------------------------------------------
/* Criar um novo grafo a partir de um arquivo */
void TEmergencyHelper::AbrirGrafo() {
	// Se ocorrer algum erro durante a escolha do nome de um arquivo, não faz nada e sai.
	if (OpenDialog->Execute() == false) {
    	return;
    }

	Graph = new Grafo(OpenDialog->FileName.c_str());
	CalcularEscala();
	ImagemGrafo->Canvas->FloodFill(0, 0, clBtnFace, fsBorder);

    for (int i = 1; i <= Graph->pegarNumeroArestas(); i++) {
		TPoint origem, destino;
		int *aresta;
		aresta = Graph->pegarAresta(i);
		origem = Graph->pegarPonto(aresta[0]);
		destino = Graph->pegarPonto(aresta[1]);
		origem.x *= escala;
		origem.y *= escala;
		destino.x *= escala;
		destino.y *= escala;
		DesenharAresta(origem, destino, clBlack);
		delete[] aresta;
	}

	for (int i = 1; i <= Graph->pegarNumeroVertices(); i++) {
		TPoint p = Graph->pegarPonto(i);
		p.x *= escala;
		p.y *= escala;
		DesenharCirculo(clWhite, p, i);
	}

	Graph->aplicarFloyd();

    MenuComando->Enabled = true;

	ToolButtonDistribuirAmbulancias->Enabled = true;
	MenuComandoDistribuirAmbulancias->Enabled = true;

	ToolButtonConstruirHospitais->Enabled = true;
	MenuComandoConstruirHospitais->Enabled = true;

	ToolButtonGerarAcidente->Enabled = true;
	MenuComandoGerarAcidente->Enabled = true;

	ToolButtonAcharMelhorRota->Enabled = false;
	MenuComandoAcharMelhorRota->Enabled = false;

	ToolButtonFechar->Enabled = true;
	MenuArquivoFechar->Enabled = true;

	ToolButtonAbrir->Enabled = false;
	MenuArquivoAbrir->Enabled = false;
}

//---------------------------------------------------------------------------
void TEmergencyHelper::GerarAcidente() {
	if (!ToolButtonConstruirHospitais->Enabled && !ToolButtonDistribuirAmbulancias->Enabled) {
		ToolButtonAcharMelhorRota->Enabled = true;
		MenuComandoAcharMelhorRota->Enabled = true;
	}

	randomize();
	do
		acidente = rand() % (Graph->pegarNumeroVertices() + 1);
	while (acidente == 0);

    TPoint p = Graph->pegarPonto(acidente);
	p.x *= escala;
	p.y *= escala;
	DesenharQuadrado(clYellow, p, acidente);
}

//---------------------------------------------------------------------------
void TEmergencyHelper::FecharGrafo() {
	ImagemGrafo->Canvas->FloodFill(0, 0, clBtnFace, fsBorder);

	if (Graph != NULL) {
		delete Graph;
		Graph = NULL;
	}

	acidente = 0;
	escala = 0;

	ToolButtonDistribuirAmbulancias->Enabled = false;
	MenuComandoDistribuirAmbulancias->Enabled = false;

	ToolButtonConstruirHospitais->Enabled = false;
    MenuComandoConstruirHospitais->Enabled = false;

	ToolButtonGerarAcidente->Enabled = false;
	MenuComandoGerarAcidente->Enabled = false;

    ToolButtonAcharMelhorRota->Enabled = false;
	MenuComandoAcharMelhorRota->Enabled = false;

    ToolButtonFechar->Enabled = false;
	MenuArquivoFechar->Enabled = false;

	ToolButtonAbrir->Enabled = true;
	MenuArquivoAbrir->Enabled = true;

	MenuComando->Enabled = false;
}

//---------------------------------------------------------------------------
void TEmergencyHelper::DistribuirAmbulancias() {
	ToolButtonDistribuirAmbulancias->Enabled = false;
	MenuComandoDistribuirAmbulancias->Enabled = false;

	if (!ToolButtonConstruirHospitais->Enabled && acidente != 0) {
		ToolButtonAcharMelhorRota->Enabled = true;
		MenuComandoAcharMelhorRota->Enabled = true;
	}

	Graph->gerarCentros(3);

	for (int i = 1; i <= Graph->pegarNumeroCentros(); i++) {
		int ambulancia;
		ambulancia = Graph->pegarCentro(i);
		TPoint p = Graph->pegarPonto(ambulancia);
		p.x *= escala;
		p.y *=escala;
		DesenharQuadrado(clLime, p, ambulancia);
	}
}

//---------------------------------------------------------------------------
void TEmergencyHelper::ConstruirHospitais() {
	ToolButtonConstruirHospitais->Enabled = false;
	MenuComandoConstruirHospitais->Enabled = false;

	Graph->gerarCentrosEmergencia(4);

	if (!ToolButtonDistribuirAmbulancias->Enabled && acidente != 0) {
		ToolButtonAcharMelhorRota->Enabled = true;
		MenuComandoAcharMelhorRota->Enabled = true;
	}

	for (int i = 1; i <= Graph->pegarNumeroCentrosEmergencia(); i++) {
		int hospital;
		hospital = Graph->pegarCentroEmergencia(i);
		TPoint p = Graph->pegarPonto(hospital);
		p.x *= escala;
		p.y *= escala;
		DesenharCirculo(clBlue, p, hospital);
	}
}

//---------------------------------------------------------------------------
void TEmergencyHelper::DesenharQuadrado(TColor cor, TPoint p, AnsiString texto) {
	TColor cor_antiga;
	cor_antiga = ImagemGrafo->Canvas->Brush->Color;
	p.x += 10;
	p.y += 10;
	ImagemGrafo->Canvas->MoveTo(p.x , p.y);
	ImagemGrafo->Canvas->Brush->Color = cor;
	ImagemGrafo->Canvas->Rectangle(p.x - 5, p.y - 5, p.x + 5, p.y  + 5);
	ImagemGrafo->Canvas->TextFlags = ETO_OPAQUE;
	if (StrToInt(texto) < 10) {
		ImagemGrafo->Canvas->TextOut(p.x - 3, p.y - 6, texto);
	} else {
		ImagemGrafo->Canvas->TextOut(p.x - 6, p.y - 6, texto);
	}
	ImagemGrafo->Canvas->Brush->Color = cor_antiga;
}

//---------------------------------------------------------------------------
void TEmergencyHelper::DesenharCirculo(TColor cor, TPoint p, AnsiString texto) {
	TColor cor_antiga;
	cor_antiga = ImagemGrafo->Canvas->Brush->Color;
	p.x += 10;
	p.y += 10;
	ImagemGrafo->Canvas->MoveTo(p.x, p.y);
	ImagemGrafo->Canvas->Brush->Color = cor;
	ImagemGrafo->Canvas->Ellipse(p.x - 9, p.y - 9, p.x + 9, p.y + 9);
	ImagemGrafo->Canvas->TextFlags = ETO_OPAQUE;
	if (StrToInt(texto) < 10) {
		ImagemGrafo->Canvas->TextOut(p.x - 3, p.y - 6, texto);
	} else {
		ImagemGrafo->Canvas->TextOut(p.x - 6, p.y - 6, texto);
	}
	ImagemGrafo->Canvas->Brush->Color = cor_antiga;
}

//---------------------------------------------------------------------------
void TEmergencyHelper::DesenharAresta(TPoint origem, TPoint destino, TColor cor) {
	TColor cor_antiga;
	cor_antiga = ImagemGrafo->Canvas->Pen->Color;
	if (origem.x == destino.x && origem.y == destino.y)
		return;
	origem.x += 10;
	origem.y += 10;
	destino.x += 10;
	destino.y += 10;
	ImagemGrafo->Canvas->MoveTo(origem.x, origem.y);
	ImagemGrafo->Canvas->Pen->Color = cor;
	ImagemGrafo->Canvas->LineTo(destino.x , destino.y);
	DesenharSeta(origem, destino);
	ImagemGrafo->Canvas->Pen->Color = cor_antiga;
}

//---------------------------------------------------------------------------
void TEmergencyHelper::DesenharSeta(TPoint origem, TPoint destino) {
/*
	double X0, Y0, X1, Y1, X2, Y2, hipotenusa, ux, uy, tangente;

        ux = y11 - y1;
        uy = x11 - x1;
        tangente = uy / (ux + 0.00000001);
	hipotenusa = sqrt(pow(uy, 2) + pow(ux, 2));
        uy = sqrt( pow(hipotenusa - 9, 2) / (1 + pow(tangente,2)) );
        ux = uy * tangente;
	X0 = x1 - ux;
       	Y0 = y1 - uy;

        tangente = tan( PI / 3 );
        uy = sqrt( pow(hipotenusa - 5, 2 ) / (1 + pow(tangente, 2)) );
        ux = uy * tangente;
	X1 = X0 + ux;
	Y1 = X0 - uy;

	X2 = X0 - ux;
	Y2 = Y0 - uy;

	TPoint points[3];
	points[0] = Point(X0, Y0);
	points[1] = Point(X1, Y1);
	points[2] = Point(X2, Y2);
	ImagemGrafo->Canvas->Polygon(points, 2);
*/
	DesenharSetaGambiarra(destino.x, destino.y, origem.x, origem.y);
}

//---------------------------------------------------------------------------
void TEmergencyHelper::DesenharSetaGambiarra(float x1, float y1, float x11, float y11) {
	float
		X1,Y1,X2,Y2,
		ux = x11-x1,
		uy = y11-y1,
		d = 15,
		lambda = d/sqrt(ux*ux+uy*uy+0.000001),
		X = x1+lambda*ux,
		Y = y1+lambda*uy;

        d = 5;
	lambda = d/sqrt(ux*ux+uy*uy+0.000001);
	X1 = X-lambda*uy;
	Y1 = Y+lambda*ux;
	X2 = X+lambda*uy;
	Y2 = Y-lambda*ux;
	TPoint points[3];
	points[0] = Point(x1,y1);
	points[1] = Point(X1,Y1);
	points[2] = Point(X2,Y2);
	ImagemGrafo->Canvas->Polygon(points, 2);
}

//---------------------------------------------------------------------------
void AbrirFormularioSobre() {
	FormSobre->ShowModal();
}

//---------------------------------------------------------------------------
void TEmergencyHelper::CalcularEscala() {
	double maxX = 0, maxY = 0;
	TPoint p;

	for (int i = 1; i <= Graph->pegarNumeroVertices(); i++) {
		p = Graph->pegarPonto(i);
		if (p.x > maxX) {
			maxX = p.x;
        }
		if (p.y > maxY) {
			maxY = p.y;
        }
    }
    maxX = (ImagemGrafo->Width - 20) / maxX;
    maxY = (ImagemGrafo->Height - 20) / maxY;
    escala = min(maxX, maxY);
}

//---------------------------------------------------------------------------
void TEmergencyHelper::AcharMelhorRota() {
	int i, j, k, *caminho1, *caminho2;
	caminho1 = NULL;
	caminho2 = NULL;
	i = Graph->acharCentroMaisProximo(acidente);
	j = acidente;
	k = Graph->acharCentroEmergenciaMaisProximo(acidente);

	if (j == k)
		return;

	int count = 0;
	if (i != j) {
		caminho1 = Graph->acharMelhorCaminho(i, j);
        do {
			TPoint p1, p2;
			p1 = Graph->pegarPonto(caminho1[count]);
			p2 = Graph->pegarPonto(caminho1[count+1]);
			p1.x *= escala;
			p1.y *= escala;
			p2.x *= escala;
			p2.y *= escala;
			DesenharAresta(p1, p2, clLime);
			count++;
		} while (caminho1[count] != j);
	}

	count = 0;
    if (j != k) {
		caminho2 = Graph->acharMelhorCaminho(j, k);
		do {
			TPoint p1, p2;
			p1 = Graph->pegarPonto(caminho2[count]);
			p2 = Graph->pegarPonto(caminho2[count+1]);
			p1.x *= escala;
			p1.y *= escala;
			p2.x *= escala;
			p2.y *= escala;
			DesenharAresta(p1, p2, clBlue);
			count++;
		} while (caminho2[count] != k);
	}

	if (caminho1 != NULL)
		delete[] caminho1;
    if (caminho2 != NULL)
		delete[] caminho2;
}

//---------------------------------------------------------------------------
void __fastcall TEmergencyHelper::MenuArquivoAbrirClick(TObject *Sender)
{
	AbrirGrafo();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::MenuArquivoSairClick(TObject *Sender)
{
	if (Graph != NULL) {
		delete Graph;
		Graph = NULL;
	}
	Application->Terminate();
}
//---------------------------------------------------------------------------


void __fastcall TEmergencyHelper::MenuComandoGerarAcidenteClick(TObject *Sender)
{
	GerarAcidente();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::ToolButtonAbrirClick(TObject *Sender)
{
	AbrirGrafo();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::MenuArquivoFecharClick(TObject *Sender)
{
	FecharGrafo();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::ToolButtonFecharClick(TObject *Sender)
{
	FecharGrafo();
}
//---------------------------------------------------------------------------


void __fastcall TEmergencyHelper::MenuComandoDistribuirAmbulanciasClick(TObject *Sender)
{
	DistribuirAmbulancias();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::MenuComandoConstruirHospitaisClick(TObject *Sender)
{
	ConstruirHospitais();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::MenuComandoAcharMelhorRotaClick(TObject *Sender)
{
	AcharMelhorRota();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::ToolButtonDistribuirAmbulanciasClick(TObject *Sender)
{
	DistribuirAmbulancias();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::ToolButtonConstruirHospitaisClick(TObject *Sender)
{
	ConstruirHospitais();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::ToolButtonGerarAcidenteClick(TObject *Sender)
{
	GerarAcidente();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::ToolButtonAcharMelhorRotaClick(TObject *Sender)
{
	AcharMelhorRota();
}
//---------------------------------------------------------------------------


void __fastcall TEmergencyHelper::ToolButtonAjudaClick(TObject *Sender)
{
	AbrirFormularioSobre();
}
//---------------------------------------------------------------------------

void __fastcall TEmergencyHelper::MenuAjudaSobreClick(TObject *Sender)
{
	AbrirFormularioSobre();
}
//---------------------------------------------------------------------------
