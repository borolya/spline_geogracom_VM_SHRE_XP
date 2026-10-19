object ftype: Tftype
  Left = 440
  Top = 401
  BorderStyle = bsDialog
  Caption = #1047#1072#1076#1072#1081#1090#1077' '#1087#1080#1082#1077#1090#1072#1078' '#1085#1072#1095#1072#1083#1100#1085#1086#1081' '#1090#1086#1095#1082#1080
  ClientHeight = 161
  ClientWidth = 373
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 29
    Top = 32
    Width = 201
    Height = 22
    AutoSize = False
    Caption = '     '#1055#1080#1082#1077#1090'                            '#1087#1083#1102#1089'  '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clCaptionText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 119
    Top = 56
    Width = 38
    Height = 21
    AutoSize = False
    Caption = '  +'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Button1: TButton
    Left = 64
    Top = 112
    Width = 49
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
    OnClick = Button1Click
  end
  object Edit1: TEdit
    Left = 42
    Top = 57
    Width = 76
    Height = 24
    AutoSize = False
    TabOrder = 1
    Text = '        0'
  end
  object Edit2: TEdit
    Left = 162
    Top = 56
    Width = 69
    Height = 26
    AutoSize = False
    TabOrder = 2
    Text = '     0,0'
  end
end
