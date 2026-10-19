object Form2: TForm2
  Left = 308
  Top = 234
  Width = 367
  Height = 169
  Caption = #1044#1086#1073#1072#1074#1083#1077#1085#1080#1077' '#1075#1088#1072#1085#1080#1094#1099
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
    Left = 40
    Top = 48
    Width = 92
    Height = 16
    Caption = #1058#1080#1087' '#1101#1083#1077#1084#1077#1085#1090#1072
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object List: TComboBox
    Left = 128
    Top = 40
    Width = 145
    Height = 21
    ItemHeight = 13
    TabOrder = 0
    Items.Strings = (
      #1055#1088#1103#1084#1072#1103
      #1054#1082#1088#1091#1078#1085#1086#1089#1090#1100
      #1050#1083#1086#1090#1086#1080#1076#1072)
  end
  object Ok: TButton
    Left = 128
    Top = 80
    Width = 75
    Height = 25
    Caption = 'Ok'
    TabOrder = 1
    OnClick = OkClick
  end
  object Cancel: TButton
    Left = 208
    Top = 80
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = CancelClick
  end
end
