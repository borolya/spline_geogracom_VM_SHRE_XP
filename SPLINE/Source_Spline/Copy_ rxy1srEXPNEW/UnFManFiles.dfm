object FManFiles: TFManFiles
  Left = 303
  Top = 165
  Width = 316
  Height = 391
  Caption = #1042#1099#1073#1086#1088' '#1082#1088#1080#1074#1086#1081
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 0
    Top = 0
    Width = 300
    Height = 16
    Align = alTop
    Alignment = taCenter
    Caption = ' '#1048#1084#1077#1085#1072' '#1082#1088#1080#1074#1099#1093':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object ListBox1: TListBox
    Left = 0
    Top = 16
    Width = 298
    Height = 265
    Columns = 3
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemHeight = 13
    Items.Strings = (
      '')
    ParentFont = False
    TabOrder = 0
    OnDblClick = ListBox1DblClick
  end
  object Button1: TButton
    Left = 112
    Top = 314
    Width = 75
    Height = 33
    Caption = #1042#1086#1079#1074#1088#1072#1090
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ModalResult = 1
    ParentFont = False
    TabOrder = 1
    OnClick = Button1Click
  end
end
