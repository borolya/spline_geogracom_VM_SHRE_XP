object fPSK: TfPSK
  Left = 324
  Top = 358
  Width = 479
  Height = 335
  Caption = 'fPSK'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object sgPSK: TStringGrid
    Left = 8
    Top = 8
    Width = 457
    Height = 289
    ColCount = 3
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
    TabOrder = 0
    OnSelectCell = sgPSKSelectCell
    OnSetEditText = sgPSKSetEditText
    ColWidths = (
      64
      142
      156)
  end
end
