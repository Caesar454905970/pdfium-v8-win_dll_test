from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import letter
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.pdfbase import pdfmetrics
from reportlab.lib import colors

def create_pdf_with_form_field(filename):
    c = canvas.Canvas(filename, pagesize=letter)
    width, height = letter

    # 注册自定义字体
    pdfmetrics.registerFont(TTFont('AlimamaFangYuan', 'AlimamaFangYuan.ttf'))

    # 设置标题
    c.setFont("AlimamaFangYuan", 12)
    c.drawString(200, height - 40, "PDF with Form Field")

    # 添加表单字段标签
    c.drawString(10, height - 80, "Name:")

    # 创建表单字段
    form = c.acroForm
    form.textfield(name='Name',
                   tooltip='Name',
                   x=50, y=height - 90,
                   width=200,
                   height=20,
                   borderStyle='inset',
                   borderColor=colors.black,
                   fillColor=colors.white,
                   textColor=colors.black,
                   forceBorder=True,
                   fontName='Helvetica',  # 使用标准字体
                   fontSize=12)

    # 保存PDF
    c.save()

create_pdf_with_form_field("output_with_form_field.pdf")
print("PDF with form field created successfully.")