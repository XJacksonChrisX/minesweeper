#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from pptx import Presentation
from pptx.util import Inches, Pt
from pptx.enum.text import PP_ALIGN
from pptx.dml.color import RGBColor

# 建立簡報
prs = Presentation()
prs.slide_width = Inches(10)
prs.slide_height = Inches(7.5)

def add_title_slide(prs, title, subtitle):
    """添加標題頁"""
    slide = prs.slides.add_slide(prs.slide_layouts[6])  # 空白版面
    background = slide.background
    fill = background.fill
    fill.solid()
    fill.fore_color.rgb = RGBColor(41, 128, 185)  # 藍色背景
    
    # 標題
    title_box = slide.shapes.add_textbox(Inches(0.5), Inches(2.5), Inches(9), Inches(1.5))
    title_frame = title_box.text_frame
    title_frame.text = title
    title_p = title_frame.paragraphs[0]
    title_p.font.size = Pt(60)
    title_p.font.bold = True
    title_p.font.color.rgb = RGBColor(255, 255, 255)
    title_p.alignment = PP_ALIGN.CENTER
    
    # 副標題
    subtitle_box = slide.shapes.add_textbox(Inches(0.5), Inches(4.2), Inches(9), Inches(1))
    subtitle_frame = subtitle_box.text_frame
    subtitle_frame.text = subtitle
    subtitle_p = subtitle_frame.paragraphs[0]
    subtitle_p.font.size = Pt(28)
    subtitle_p.font.color.rgb = RGBColor(255, 255, 255)
    subtitle_p.alignment = PP_ALIGN.CENTER

def add_content_slide(prs, title, content_list):
    """添加內容頁"""
    slide = prs.slides.add_slide(prs.slide_layouts[6])  # 空白版面
    background = slide.background
    fill = background.fill
    fill.solid()
    fill.fore_color.rgb = RGBColor(236, 240, 241)  # 淺灰色背景
    
    # 標題
    title_box = slide.shapes.add_textbox(Inches(0.5), Inches(0.5), Inches(9), Inches(0.8))
    title_frame = title_box.text_frame
    title_frame.text = title
    title_p = title_frame.paragraphs[0]
    title_p.font.size = Pt(44)
    title_p.font.bold = True
    title_p.font.color.rgb = RGBColor(41, 128, 185)
    
    # 內容
    content_box = slide.shapes.add_textbox(Inches(0.8), Inches(1.5), Inches(8.4), Inches(5.5))
    text_frame = content_box.text_frame
    text_frame.word_wrap = True
    
    for i, item in enumerate(content_list):
        if i > 0:
            text_frame.add_paragraph()
        p = text_frame.paragraphs[i]
        p.text = item
        p.font.size = Pt(20)
        p.font.color.rgb = RGBColor(52, 73, 94)
        p.space_before = Pt(8)
        p.space_after = Pt(8)
        p.level = 0

def add_two_column_slide(prs, title, left_title, left_items, right_title, right_items):
    """添加雙列內容頁"""
    slide = prs.slides.add_slide(prs.slide_layouts[6])
    background = slide.background
    fill = background.fill
    fill.solid()
    fill.fore_color.rgb = RGBColor(236, 240, 241)
    
    # 標題
    title_box = slide.shapes.add_textbox(Inches(0.5), Inches(0.5), Inches(9), Inches(0.8))
    title_frame = title_box.text_frame
    title_frame.text = title
    title_p = title_frame.paragraphs[0]
    title_p.font.size = Pt(44)
    title_p.font.bold = True
    title_p.font.color.rgb = RGBColor(41, 128, 185)
    
    # 左列標題
    left_title_box = slide.shapes.add_textbox(Inches(0.5), Inches(1.5), Inches(4.5), Inches(0.4))
    left_title_frame = left_title_box.text_frame
    left_title_frame.text = left_title
    left_title_p = left_title_frame.paragraphs[0]
    left_title_p.font.size = Pt(22)
    left_title_p.font.bold = True
    left_title_p.font.color.rgb = RGBColor(52, 73, 94)
    
    # 左列內容
    left_box = slide.shapes.add_textbox(Inches(0.5), Inches(2.0), Inches(4.5), Inches(5))
    left_frame = left_box.text_frame
    left_frame.word_wrap = True
    for i, item in enumerate(left_items):
        if i > 0:
            left_frame.add_paragraph()
        p = left_frame.paragraphs[i]
        p.text = item
        p.font.size = Pt(18)
        p.font.color.rgb = RGBColor(52, 73, 94)
        p.space_before = Pt(4)
        p.space_after = Pt(4)
    
    # 右列標題
    right_title_box = slide.shapes.add_textbox(Inches(5.2), Inches(1.5), Inches(4.3), Inches(0.4))
    right_title_frame = right_title_box.text_frame
    right_title_frame.text = right_title
    right_title_p = right_title_frame.paragraphs[0]
    right_title_p.font.size = Pt(22)
    right_title_p.font.bold = True
    right_title_p.font.color.rgb = RGBColor(52, 73, 94)
    
    # 右列內容
    right_box = slide.shapes.add_textbox(Inches(5.2), Inches(2.0), Inches(4.3), Inches(5))
    right_frame = right_box.text_frame
    right_frame.word_wrap = True
    for i, item in enumerate(right_items):
        if i > 0:
            right_frame.add_paragraph()
        p = right_frame.paragraphs[i]
        p.text = item
        p.font.size = Pt(18)
        p.font.color.rgb = RGBColor(52, 73, 94)
        p.space_before = Pt(4)
        p.space_after = Pt(4)

# 第 1 頁：組別
add_title_slide(prs, "組別", "第 20 組")

# 第 2 頁：系級班級
add_title_slide(prs, "系級班級", "資訊工程系 3B")

# 第 3 頁：成員資訊
add_content_slide(prs, "成員資訊", [
    "組長：詹書豪 (B2230041)",
    "      負責：專案規劃與整合、程式設計與測試",
    "",
    "組員：李程絛 (B2230319)",
    "",
    "組員：林元皓 (B3401214)"
])

# 第 4 頁：小專題題目
add_title_slide(prs, "小專題題目", "踩地雷遊戲\n(Minesweeper Game)")

# 第 5 頁：程式介紹 - UML 與遊戲規則
add_content_slide(prs, "程式介紹 (UML 圖與操作說明)", [
    "系統架構設計：",
    "  • Cell 類別 - 代表棋盤上的單一格子",
    "  • Board 類別 - 管理棋盤狀態和地雷位置",
    "  • Game 類別 - 控制遊戲主邏輯",
    "  • Display 類別 - 負責棋盤顯示",
    "",
    "遊戲規則：選擇難度 → 揭露格子 → 避開地雷 → 標記疑似地雷 → 贏得遊戲"
])

# 第 6 頁：程式如何安裝執行（圖文講解）
add_content_slide(prs, "程式如何安裝執行", [
    "快速開始（Windows 用戶）：",
    "1. 進入 Releases 頁面",
    "2. 下載最新的 minesweeper.exe",
    "3. 雙擊執行",
    "",
    "Linux / macOS 用戶：",
    "1. 下載並解壓 ZIP 檔案",
    "2. 進入資料夾執行 ./minesweeper"
])

# 第 7 頁：程式畫面截圖
add_content_slide(prs, "程式畫面示範", [
    "遊戲開始畫面：",
    "  ┌─────────────────────┐",
    "  │  踩地雷遊戲        │",
    "  │  1. 簡單  2. 中等  3. 困難",
    "  │  請選擇難度：     │",
    "  └─────────────────────┘",
    "",
    "遊戲中顯示：棋盤格子、地雷數量、遊戲狀態",
    "遊戲結束：勝利或失敗訊息顯示"
])

# 第 8 頁：分工資訊
add_content_slide(prs, "分工資訊", [
    "詹書豪 (B2230041) - 組長",
    "  • 整體專案規劃與架構設計",
    "  • Cell 類別開發",
    "  • Board 類別開發",
    "  • 程式整合與測試",
    "",
    "李程絛 (B2230319) & 林元皓 (B3401214)",
    "  • 輔助開發和測試"
])

# 第 9 頁：GitHub 連結
add_title_slide(prs, "GitHub 連結", 
    "Repository:\nhttps://github.com/XJacksonChrisX/minesweeper\n\nDownload: Releases 頁面")

# 保存簡報
prs.save('/workspaces/game/踩地雷遊戲_專題報告.pptx')
print("✓ PPT 檔案已成功重新生成：踩地雷遊戲_專題報告.pptx")
print("✓ 共 9 頁，按照要求順序排版")
