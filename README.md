# 使用谷歌的pdf库，测试功能，并打包为ddl库

## 中文总结这里面的函数功能

```
以下是 include/fpdf_edit.h 文件中部分函数的功能总结：
FPDFPage_HasTransparency
检查页面是否包含透明度。
参数：page - 页面句柄。
返回值：如果页面包含透明度，返回 TRUE。
FPDFPage_GenerateContent
生成页面的内容。
参数：page - 页面句柄。
返回值：成功时返回 TRUE。
说明：在保存页面到文件或重新加载页面之前，必须调用此函数，否则对页面的更改将丢失。
FPDFPageObj_Destroy
销毁页面对象并释放其资源。
参数：page_object - 页面对象句柄。
说明：如果页面对象未添加到页面或注释中，必须调用此函数。
FPDFPageObj_HasTransparency
检查页面对象是否包含透明度。
参数：page_object - 页面对象句柄。
返回值：如果页面对象包含透明度，返回 TRUE。
5. FPDFPageObj_GetType
获取页面对象的类型。
参数：page_object - 页面对象句柄。
返回值：成功时返回 FPDF_PAGEOBJ_ 值，错误时返回 FPDF_PAGEOBJ_UNKNOWN。
6. FPDFPageObj_Transform
通过给定的矩阵变换页面对象。
参数：page_object - 页面对象句柄，a, b, c, d, e, f - 矩阵值。
说明：矩阵用于缩放、旋转、剪切和平移页面对象。
FPDFPageObj_TransformF
通过给定的矩阵变换页面对象（实验性 API）。
参数：page_object - 页面对象句柄，matrix - 变换矩阵。
返回值：成功时返回 TRUE。
说明：改进版的 FPDFPageObj_Transform()，避免不必要的双精度到浮点转换。
FPDFPageObj_GetMatrix
获取页面对象的变换矩阵（实验性 API）。
参数：page_object - 页面对象句柄，matrix - 用于接收矩阵值的结构体指针。
返回值：成功时返回 TRUE。
9. FPDFPageObj_SetMatrix
设置页面对象的变换矩阵（实验性 API）。
参数：page_object - 页面对象句柄，matrix - 包含矩阵值的结构体指针。
返回值：成功时返回 TRUE。
10. FPDFPage_TransformAnnots
变换页面中的所有注释。
参数：page - 页面句柄，a, b, c, d, e, f - 矩阵值。
说明：矩阵用于缩放、旋转、剪切和平移页面注释。
FPDFPageObj_NewImageObj
创建一个新的图像对象。
参数：document - 文档句柄。
返回值：返回新的图像对象句柄。
FPDFText_LoadFont
从数据流中加载字体对象。
参数：document - 文档句柄，data - 字体数据流，size - 字体数据大小，font_type - 字体类型，cid - 是否为 CID 字体。
返回值：成功时返回字体对象句柄，失败时返回 NULL。
13. FPDFText_SetText
设置文本对象的文本内容。
参数：text_object - 文本对象句柄，text - UTF-16LE 编码的字符串。
返回值：成功时返回 TRUE。
FPDFImageObj_GetBitmap
获取图像对象的位图光栅化。
参数：image_object - 图像对象句柄。
返回值：返回位图，调用者拥有返回的位图，使用 FPDFBitmap_Destroy() 释放。
这些函数提供了对 PDF 页面和对象的各种操作，包括创建、变换、检查透明度、设置文本和图像等。
```

## 中文总结这里面的函数功能

```
以下是 include/fpdf_annot.h 文件中部分函数的功能总结：
FPDFAnnot_AddInkStroke
向注释的墨迹列表中添加一个新的墨迹笔画。
参数：annot - 注释句柄，points - 点数组，point_count - 点的数量。
返回值：成功时返回新墨迹笔画在墨迹列表中的0基索引，失败时返回-1。
2. FPDFAnnot_RemoveInkList
移除注释中的墨迹列表。
参数：annot - 注释句柄。
返回值：成功时返回 TRUE，失败时返回 FALSE。
FPDFAnnot_AppendObject
将对象添加到注释中。
参数：annot - 注释句柄，obj - 要添加的对象句柄。
返回值：成功时返回 TRUE。
FPDFAnnot_GetObjectCount
获取注释中的对象总数。
参数：annot - 注释句柄。
返回值：注释中的对象数量。
5. FPDFAnnot_GetObject
获取注释中指定索引的对象。
参数：annot - 注释句柄，index - 对象索引。
返回值：对象句柄，失败时返回 NULL。
FPDFAnnot_RemoveObject
移除注释中指定索引的对象。
参数：annot - 注释句柄，index - 要移除的对象索引。
返回值：成功时返回 TRUE。
7. FPDFAnnot_SetColor
设置注释的颜色。
参数：annot - 注释句柄，type - 颜色类型，R, G, B - RGB 值，A - 不透明度。
返回值：成功时返回 TRUE。
FPDFAnnot_GetColor
获取注释的颜色。
参数：annot - 注释句柄，type - 颜色类型，R, G, B - RGB 值缓冲区，A - 不透明度缓冲区。
返回值：成功时返回 TRUE。
9. FPDFAnnot_GetRect
获取注释的矩形区域。
参数：annot - 注释句柄，rect - 接收矩形区域的结构体指针。
返回值：成功时返回 TRUE。
FPDFAnnot_GetVertices
获取多边形或折线注释的顶点。
参数：annot - 注释句柄，buffer - 存放顶点的缓冲区，length - 缓冲区长度。
返回值：顶点数量。
11. FPDFAnnot_GetInkListCount
获取墨迹注释的墨迹列表中的路径数量。
参数：annot - 注释句柄。
返回值：路径数量。
12. FPDFAnnot_GetInkListPath
获取墨迹注释的墨迹列表中的指定路径。
参数：annot - 注释句柄，path_index - 路径索引，buffer - 存放路径点的缓冲区，length - 缓冲区长度。
返回值：路径点数量。
FPDFAnnot_GetLine
获取线条注释的起点和终点坐标。
参数：annot - 注释句柄，start - 起点，end - 终点。
返回值：成功时返回 TRUE。
FPDFAnnot_SetBorder
设置注释的边框特性（圆角矩形）。
参数：annot - 注释句柄，horizontal_radius - 水平半径，vertical_radius - 垂直半径，border_width - 边框宽度。
返回值：成功时返回 TRUE。
FPDFAnnot_GetBorder
获取注释的边框特性（圆角矩形）。
参数：annot - 注释句柄，horizontal_radius - 水平半径，vertical_radius - 垂直半径，border_width - 边框宽度。
返回值：成功时返回 TRUE。
16. FPDFAnnot_GetFormAdditionalActionJavaScript
获取注释的附加动作的 JavaScript。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄，event - 事件类型，buffer - 存放 JavaScript 的缓冲区，buflen - 缓冲区长度。
返回值：JavaScript 字符串的长度（字节数）。
FPDFAnnot_HasKey
检查注释的字典中是否包含指定的键。
参数：annot - 注释句柄，key - 键（UTF-8 编码）。
返回值：如果键存在，返回 TRUE。
18. FPDFAnnot_GetValueType
获取注释字典中指定键的值类型。
参数：annot - 注释句柄，key - 键（UTF-8 编码）。
返回值：字典值的类型。
FPDFAnnot_SetStringValue
设置注释字典中指定键的字符串值。
参数：annot - 注释句柄，key - 键（UTF-8 编码），value - 字符串值（UTF-16LE 编码）。
返回值：成功时返回 TRUE。
20. FPDFAnnot_GetStringValue
获取注释字典中指定键的字符串值。
参数：annot - 注释句柄，key - 键（UTF-8 编码），buffer - 存放字符串值的缓冲区（UTF-16LE 编码），buflen - 缓冲区长度。
返回值：字符串值的长度（字节数）。
21. FPDFAnnot_GetNumberValue
获取注释字典中指定键的浮点值。
参数：annot - 注释句柄，key - 键（UTF-8 编码），value - 接收浮点值的指针。
返回值：成功时返回 TRUE。
22. FPDFAnnot_SetAP
设置注释字典中指定外观模式的外观字符串。
参数：annot - 注释句柄，appearanceMode - 外观模式，value - 外观字符串（UTF-16LE 编码）。
返回值：成功时返回 TRUE。
FPDFAnnot_GetAP
获取注释字典中指定外观模式的外观字符串。
参数：annot - 注释句柄，appearanceMode - 外观模式，buffer - 存放外观字符串的缓冲区（UTF-16LE 编码），buflen - 缓冲区长度。
返回值：外观字符串的长度（字节数）。
FPDFAnnot_GetLinkedAnnot
获取注释字典中指定键对应的链接注释。
参数：annot - 注释句柄，key - 键（UTF-8 编码）。
返回值：链接注释的句柄，失败时返回 NULL。
FPDFAnnot_GetFlags
获取注释的标志。
参数：annot - 注释句柄。
返回值：注释的标志。
FPDFAnnot_SetFlags
设置注释的标志。
参数：annot - 注释句柄，flags - 标志值。
返回值：成功时返回 TRUE。
27. FPDFAnnot_GetFormFieldFlags
获取交互式表单注释的标志。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄。
返回值：交互式表单注释的标志。
28. FPDFAnnot_GetFormFieldAtPoint
获取包含指定点的交互式表单注释。
参数：hHandle - 表单填充模块句柄，page - 页面句柄，point - 点坐标。
返回值：交互式表单注释的句柄，失败时返回 NULL。
29. FPDFAnnot_GetFormFieldName
获取交互式表单注释的名称。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄，buffer - 存放名称字符串的缓冲区（UTF-16LE 编码），buflen - 缓冲区长度。
返回值：名称字符串的长度（字节数）。
FPDFAnnot_GetFormFieldAlternateName
获取交互式表单注释的备用名称。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄，buffer - 存放备用名称字符串的缓冲区（UTF-16LE 编码），buflen - 缓冲区长度。
返回值：备用名称字符串的长度（字节数）。
FPDFAnnot_GetFormFieldType
获取交互式表单注释的字段类型。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄。
返回值：字段类型，失败时返回 -1。
32. FPDFAnnot_GetFormFieldValue
获取交互式表单注释的值。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄，buffer - 存放值字符串的缓冲区（UTF-16LE 编码），buflen - 缓冲区长度。
返回值：值字符串的长度（字节数）。
33. FPDFAnnot_GetOptionCount
获取交互式表单注释的 "Opt" 字典中的选项数量。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄。
返回值：选项数量，失败时返回 -1。
FPDFAnnot_GetOptionLabel
获取交互式表单注释的 "Opt" 字典中指定索引的选项标签。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄，index - 选项索引，buffer - 存放标签字符串的缓冲区（UTF-16LE 编码），buflen - 缓冲区长度。
返回值：标签字符串的长度（字节数）。
35. FPDFAnnot_IsOptionSelected
判断交互式表单注释的 "Opt" 字典中指定索引的选项是否被选中。
参数：handle - 表单填充模块句柄，annot - 注释句柄，index - 选项索引。
返回值：如果选项被选中，返回 TRUE。
36. FPDFAnnot_GetFormControlCount
获取交互式表单注释的控制组中的注释数量。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄。
返回值：控制组中的注释数量，失败时返回 -1。
37. FPDFAnnot_GetFormControlIndex
获取交互式表单注释在其控制组中的索引。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄。
返回值：注释在控制组中的索引，失败时返回 -1。
FPDFAnnot_GetFormFieldExportValue
获取交互式表单注释的导出值。
参数：hHandle - 表单填充模块句柄，annot - 注释句柄，buffer - 存放导出值字符串的缓冲区（UTF-16LE 编码），buflen - 缓冲区长度。
返回值：导出值字符串的长度（字节数）。
FPDFAnnot_SetURI
为链接注释添加 URI 动作。
参数：annot - 链接注释句柄，uri - URI 字符串（7-bit ASCII 编码）。
返回值：成功时返回 TRUE。
FPDFAnnot_GetFileAttachment
获取文件注释的附件。
参数：annot - 文件注释句柄。
返回值：附件句柄，失败时返回 NULL。
41. FPDFAnnot_AddFileAttachment
向文件注释添加嵌入文件。
参数：annot - 文件注释句柄，name - 新附件的名称。
返回值：新附件对象的句柄，失败时返回 NULL。
这些函数提供了对 PDF 注释的各种操作，包括添加、移除、设置颜色、获取矩形区域、获取顶点、设置边框、获取和设置字典值、获取和设置外观字符串、获取链接注释、获取和设置标志、获取表单字段信息等。
```

## 中文总结这里面的函数功能

```
以下是 include/fpdf_attachment.h 文件中部分函数的功能总结：
FPDFDoc_GetAttachmentCount
获取文档中嵌入文件的数量。
参数：document - 文档句柄。
返回值：文档中嵌入文件的数量。
2. FPDFDoc_AddAttachment
向文档中添加一个嵌入文件。
参数：document - 文档句柄，name - 新附件的名称（UTF-16LE 编码）。
返回值：新附件对象的句柄，失败时返回 NULL。
3. FPDFDoc_GetAttachment
获取文档中指定索引的嵌入文件。
参数：document - 文档句柄，index - 嵌入文件的索引。
返回值：附件对象的句柄，失败时返回 NULL。
4. FPDFDoc_DeleteAttachment
删除文档中指定索引的嵌入文件。
参数：document - 文档句柄，index - 要删除的嵌入文件的索引。
返回值：成功时返回 TRUE。
5. FPDFAttachment_GetName
获取附件文件的名称。
参数：attachment - 附件句柄，buffer - 存放文件名的缓冲区（UTF-16LE 编码），buflen - 缓冲区长度（字节数）。
返回值：文件名的长度（字节数）。
6. FPDFAttachment_HasKey
检查附件的参数字典中是否包含指定的键。
参数：attachment - 附件句柄，key - 键（UTF-8 编码）。
返回值：如果键存在，返回 TRUE。
7. FPDFAttachment_GetValueType
获取附件参数字典中指定键的值类型。
参数：attachment - 附件句柄，key - 键（UTF-8 编码）。
返回值：字典值的类型。
FPDFAttachment_SetStringValue
设置附件参数字典中指定键的字符串值。
参数：attachment - 附件句柄，key - 键（UTF-8 编码），value - 字符串值（UTF-16LE 编码）。
返回值：成功时返回 TRUE。
9. FPDFAttachment_GetStringValue
获取附件参数字典中指定键的字符串值。
参数：attachment - 附件句柄，key - 键（UTF-8 编码），buffer - 存放字符串值的缓冲区（UTF-16LE 编码），buflen - 缓冲区长度（字节数）。
返回值：字符串值的长度（字节数）。
10. FPDFAttachment_SetFile
设置附件的文件数据，覆盖现有文件数据。
参数：attachment - 附件句柄，document - 文档句柄，contents - 文件数据缓冲区，len - 文件数据长度（字节数）。
返回值：成功时返回 TRUE。
FPDFAttachment_GetFile
获取附件的文件数据。
参数：attachment - 附件句柄，buffer - 存放文件数据的缓冲区，buflen - 缓冲区长度（字节数），out_buflen - 接收文件数据大小的指针。
返回值：成功时返回 TRUE。
这些函数提供了对 PDF 文档中嵌入文件的各种操作，包括获取嵌入文件数量、添加和删除嵌入文件、获取和设置附件的名称和参数、获取和设置附件的文件数据等。
```

## 中文总结这里面的函数功能

```
以下是 include/fpdf_doc.h 文件中部分函数的功能总结：
1. FPDFBookmark_GetFirstChild
获取书签的第一个子书签或第一个顶级书签。
参数：document - 文档句柄，bookmark - 当前书签句柄（传递 NULL 获取第一个顶级书签）。
返回值：第一个子书签或顶级书签的句柄，若无子书签或顶级书签则返回 NULL。
FPDFBookmark_GetNextSibling
获取书签的下一个兄弟书签。
参数：document - 文档句柄，bookmark - 当前书签句柄。
返回值：下一个兄弟书签的句柄，若无下一个兄弟书签则返回 NULL。
3. FPDFBookmark_GetTitle
获取书签的标题。
参数：bookmark - 书签句柄，buffer - 存放标题的缓冲区，buflen - 缓冲区长度（字节数）。
返回值：标题的字节数，包括终止的 NUL 字符。
4. FPDFBookmark_GetCount
获取书签的子项数量。
参数：bookmark - 书签句柄。
返回值：子项数量，正数表示默认展开，负数表示默认折叠，0 表示无子项或无效书签。
FPDFBookmark_Find
在文档中查找指定标题的书签。
参数：document - 文档句柄，title - 要查找的书签标题（UTF-16LE 编码）。
返回值：找到的书签句柄，若未找到则返回 NULL。
6. FPDFBookmark_GetDest
获取书签关联的目标。
参数：document - 文档句柄，bookmark - 书签句柄。
返回值：目标数据的句柄，若无关联目标则返回 NULL。
FPDFBookmark_GetAction
获取书签关联的动作。
参数：bookmark - 书签句柄。
返回值：动作数据的句柄，若无关联动作则返回 NULL。
FPDFAction_GetType
获取动作的类型。
参数：action - 动作句柄。
返回值：动作类型，可能的值包括 PDFACTION_UNSUPPORTED、PDFACTION_GOTO、PDFACTION_REMOTEGOTO、PDFACTION_URI、PDFACTION_LAUNCH。
FPDFAction_GetDest
获取动作的目标。
参数：document - 文档句柄，action - 动作句柄（必须是 PDFACTION_GOTO 或 PDFACTION_REMOTEGOTO）。
返回值：目标数据的句柄，若出错则返回 NULL。
FPDFAction_GetFilePath
获取动作的文件路径。
参数：action - 动作句柄（必须是 PDFACTION_LAUNCH 或 PDFACTION_REMOTEGOTO），buffer - 存放路径字符串的缓冲区，buflen - 缓冲区长度（字节数）。
返回值：文件路径的字节数，包括终止的 NUL 字符。
FPDFAction_GetURIPath
获取动作的 URI 路径。
参数：document - 文档句柄，action - 动作句柄（必须是 PDFACTION_URI），buffer - 存放路径字符串的缓冲区，buflen - 缓冲区长度（字节数）。
返回值：URI 路径的字节数，包括终止的 NUL 字符。
FPDFDest_GetDestPageIndex
获取目标所在的页面索引。
参数：document - 文档句柄，dest - 目标句柄。
返回值：目标所在的 0 基页面索引，出错时返回 -1。
13. FPDFDest_GetView
获取目标指定的视图（适合类型）。
参数：dest - 目标句柄，pNumParams - 接收视图参数数量的指针，pParams - 存放视图参数的缓冲区（至少 4 个 FS_FLOAT 长度）。
返回值：视图类型，可能的值包括 PDFDEST_VIEW_ 常量。
14. FPDFDest_GetLocationInPage
获取目标在页面中的位置（x, y, zoom）。
参数：dest - 目标句柄，hasXVal - 是否有 x 值，hasYVal - 是否有 y 值，hasZoomVal - 是否有 zoom 值，x - x 坐标，y - y 坐标，zoom - 缩放值。
返回值：成功时返回 TRUE。
15. FPDFLink_GetLinkAtPoint
在页面的指定点查找链接。
参数：page - 页面句柄，x - x 坐标，y - y 坐标。
返回值：链接句柄，若未找到则返回 NULL。
16. FPDFLink_GetLinkZOrderAtPoint
获取页面指定点的链接的 Z 顺序。
参数：page - 页面句柄，x - x 坐标，y - y 坐标。
返回值：链接的 Z 顺序，未找到则返回 -1。
17. FPDFLink_GetDest
获取链接的目标信息。
参数：document - 文档句柄，link - 链接句柄。
返回值：目标句柄，若无关联目标则返回 NULL。
18. FPDFLink_GetAction
获取链接的动作信息。
参数：link - 链接句柄。
返回值：动作句柄，若无关联动作则返回 NULL。
FPDFLink_Enumerate
枚举页面中的所有链接注释。
参数：page - 页面句柄，start_pos - 起始位置（初始为 0），link_annot - 链接注释句柄。
返回值：成功时返回 TRUE。
20. FPDFLink_GetAnnot
获取链接注释的 FPDF_ANNOTATION 对象。
参数：page - 页面句柄，link_annot - 链接注释句柄。
返回值：FPDF_ANNOTATION 对象，失败时返回 NULL。
21. FPDFLink_GetAnnotRect
获取链接注释的矩形区域。
参数：link_annot - 链接注释句柄，rect - 注释矩形区域。
返回值：成功时返回 TRUE。
22. FPDFLink_CountQuadPoints
获取链接注释的四边形点的数量。
参数：link_annot - 链接注释句柄。
返回值：四边形点的数量。
23. FPDFLink_GetQuadPoints
获取链接注释中指定索引的四边形点。
参数：link_annot - 链接注释句柄，quad_index - 四边形点索引，quad_points - 接收四边形点的缓冲区。
返回值：成功时返回 TRUE。
FPDF_GetPageAAction
获取页面的附加动作。
参数：page - 页面句柄，aa_type - 附加动作类型。
返回值：动作数据的句柄，若无附加动作则返回 NULL。
FPDF_GetFileIdentifier
获取文档的文件标识符。
参数：document - 文档句柄，id_type - 文件标识符类型，buffer - 存放标识符的缓冲区，buflen - 缓冲区长度（字节数）。
返回值：标识符的字节数，包括终止的 NUL 字符。
26. FPDF_GetMetaText
获取文档的元数据标签内容。
参数：document - 文档句柄，tag - 要获取的标签，buffer - 存放标签内容的缓冲区，buflen - 缓冲区长度（字节数）。
返回值：标签内容的字节数，包括终止的 NUL 字符。
FPDF_GetPageLabel
获取文档中指定页面的标签。
参数：document - 文档句柄，page_index - 页面索引，buffer - 存放页面标签的缓冲区，buflen - 缓冲区长度（字节数）。
返回值：页面标签的字节数，包括终止的 NUL 字符。
这些函数提供了对 PDF 文档中书签、动作、目标、链接、页面附加动作、文件标识符、元数据标签和页面标签的各种操作。
```

## 中文总结这里面的函数功能

```
以下是 include/fpdf_text.h 文件中部分函数的功能总结：
1. FPDFText_LoadPage
准备页面中所有字符的信息。
参数：page - 页面句柄，由 FPDF_LoadPage 函数返回。
返回值：文本页面信息结构的句柄，出错时返回 NULL。
2. FPDFText_ClosePage
释放为文本页面信息结构分配的所有资源。
参数：text_page - 文本页面信息结构的句柄，由 FPDFText_LoadPage 函数返回。
返回值：无。
FPDFText_CountChars
获取页面中的字符数量。
参数：text_page - 文本页面信息结构的句柄。
返回值：页面中的字符数量，出错时返回 -1。
4. FPDFText_GetUnicode
获取页面中某个字符的 Unicode 编码。
参数：text_page - 文本页面信息结构的句柄，index - 字符的零基索引。
返回值：特定字符的 Unicode 编码，无法转换时返回 0。
FPDFText_GetTextObject
获取与给定字符关联的 FPDF_PAGEOBJECT。
参数：text_page - 文本页面信息结构的句柄，index - 字符的零基索引。
返回值：字符关联的文本对象，出错时返回 NULL。
6. FPDFText_IsGenerated
判断页面中的字符是否由 PDFium 生成。
参数：text_page - 文本页面信息结构的句柄，index - 字符的零基索引。
返回值：如果字符是由 PDFium 生成的，返回 TRUE。
FPDFText_GetCharBox
获取特定字符的边界框。
参数：text_page - 文本页面信息结构的句柄，index - 字符的零基索引，left、right、bottom、top - 接收字符框位置的指针。
返回值：成功时返回 TRUE。
FPDFText_GetLooseCharBox
获取特定字符的“松散”边界框。
参数：text_page - 文本页面信息结构的句柄，index - 字符的零基索引，rect - 接收字符框的指针。
返回值：成功时返回 TRUE。
9. FPDFText_GetMatrix
获取特定字符的变换矩阵。
参数：text_page - 文本页面信息结构的句柄，index - 字符的零基索引，matrix - 接收变换矩阵的指针。
返回值：成功时返回 TRUE。
10. FPDFText_GetCharOrigin
获取特定字符的原点。
参数：text_page - 文本页面信息结构的句柄，index - 字符的零基索引，x、y - 接收字符原点坐标的指针。
返回值：成功时返回 TRUE。
FPDFText_GetCharIndexAtPos
获取页面中某个位置的字符索引。
参数：text_page - 文本页面信息结构的句柄，x、y - 位置坐标，xTolerance、yTolerance - 容差值。
返回值：字符的零基索引，未找到时返回 -1，出错时返回 -3。
FPDFText_GetText
从页面中提取 Unicode 文本字符串。
参数：text_page - 文本页面信息结构的句柄，start_index - 起始字符索引，count - 要提取的字符数量，result - 接收提取结果的缓冲区。
返回值：写入结果缓冲区的字符数量，包括终止符。
FPDFText_CountRects
计算文本段占据的矩形区域数量，并缓存结果以供后续调用。
参数：text_page - 文本页面信息结构的句柄，start_index - 起始字符索引，count - 字符数量。
返回值：矩形区域数量，出错时返回 -1。
14. FPDFText_GetRect
获取由 FPDFText_CountRects 生成的矩形区域。
参数：text_page - 文本页面信息结构的句柄，rect_index - 矩形区域的零基索引，left、top、right、bottom - 接收矩形边界的指针。
返回值：成功时返回 TRUE。
15. FPDFText_GetBoundedText
提取页面中矩形边界内的 Unicode 文本。
参数：text_page - 文本页面信息结构的句柄，left、top、right、bottom - 矩形边界，buffer - 接收 UTF-16 值的缓冲区，buflen - 缓冲区长度。
返回值：矩形内文本的 UTF-16 值数量。
FPDFText_FindStart
开始搜索。
参数：text_page - 文本页面信息结构的句柄，findwhat - Unicode 匹配模式，flags - 选项标志，start_index - 起始字符索引。
返回值：搜索上下文的句柄。
17. FPDFText_FindNext
从页面开始到结束方向搜索。
参数：handle - 搜索上下文句柄。
返回值：找到匹配项时返回 TRUE。
FPDFText_FindPrev
从页面结束到开始方向搜索。
参数：handle - 搜索上下文句柄。
返回值：找到匹配项时返回 TRUE。
FPDFText_GetSchResultIndex
获取搜索结果的起始字符索引。
参数：handle - 搜索上下文句柄。
返回值：起始字符索引。
20. FPDFText_GetSchCount
获取搜索结果中匹配的字符数量。
参数：handle - 搜索上下文句柄。
返回值：匹配的字符数量。
FPDFText_FindClose
释放搜索上下文。
参数：handle - 搜索上下文句柄。
返回值：无。
22. FPDFLink_LoadWebLinks
准备页面中的网页链接信息。
参数：text_page - 文本页面信息结构的句柄。
返回值：页面链接信息结构的句柄，出错时返回 NULL。
FPDFLink_CountWebLinks
计算检测到的网页链接数量。
参数：link_page - 链接页面信息结构的句柄。
返回值：检测到的网页链接数量。
FPDFLink_GetURL
获取检测到的网页链接的 URL 信息。
参数：link_page - 链接页面信息结构的句柄，link_index - 链接的零基索引，buffer - 接收 URL 的缓冲区，buflen - 缓冲区长度。
返回值：URL 的 UTF-16 值数量。
FPDFLink_CountRects
计算链接的矩形区域数量。
参数：link_page - 链接页面信息结构的句柄，link_index - 链接的零基索引。
返回值：矩形区域数量。
FPDFLink_GetRect
获取链接的矩形区域边界。
参数：link_page - 链接页面信息结构的句柄，link_index - 链接的零基索引，rect_index - 矩形区域的零基索引，left、top、right、bottom - 接收矩形边界的指针。
返回值：成功时返回 TRUE。
FPDFLink_GetTextRange
获取链接的起始字符索引和字符数量。
参数：link_page - 链接页面信息结构的句柄，link_index - 链接的零基索引，start_char_index - 接收起始字符索引的指针，char_count - 接收字符数量的指针。
返回值：成功时返回 TRUE。
FPDFLink_CloseWebLinks
释放网页链接功能使用的资源。
参数：link_page - 链接页面信息结构的句柄。
返回值：无。
这些函数提供了对 PDF 文本页面和链接的各种操作，包括加载页面、获取字符信息、提取文本、搜索文本、处理网页链接等。
```

## 中文总结这里面的函数功能

```
以下是 include/fpdf_transformpage.h 文件中部分函数的功能总结：
1. FPDFPage_SetMediaBox
设置页面字典中的 "MediaBox" 条目。
参数：page - 页面句柄，left、bottom、right、top - 矩形的左、下、右、上边界。
FPDFPage_SetCropBox
设置页面字典中的 "CropBox" 条目。
参数：page - 页面句柄，left、bottom、right、top - 矩形的左、下、右、上边界。
3. FPDFPage_SetBleedBox
设置页面字典中的 "BleedBox" 条目。
参数：page - 页面句柄，left、bottom、right、top - 矩形的左、下、右、上边界。
4. FPDFPage_SetTrimBox
设置页面字典中的 "TrimBox" 条目。
参数：page - 页面句柄，left、bottom、right、top - 矩形的左、下、右、上边界。
FPDFPage_SetArtBox
设置页面字典中的 "ArtBox" 条目。
参数：page - 页面句柄，left、bottom、right、top - 矩形的左、下、右、上边界。
6. FPDFPage_GetMediaBox
获取页面字典中的 "MediaBox" 条目。
参数：page - 页面句柄，left、bottom、right、top - 接收矩形左、下、右、上边界的指针。
返回值：成功时返回 TRUE。
7. FPDFPage_GetCropBox
获取页面字典中的 "CropBox" 条目。
参数：page - 页面句柄，left、bottom、right、top - 接收矩形左、下、右、上边界的指针。
返回值：成功时返回 TRUE。
FPDFPage_GetBleedBox
获取页面字典中的 "BleedBox" 条目。
参数：page - 页面句柄，left、bottom、right、top - 接收矩形左、下、右、上边界的指针。
返回值：成功时返回 TRUE。
9. FPDFPage_GetTrimBox
获取页面字典中的 "TrimBox" 条目。
参数：page - 页面句柄，left、bottom、right、top - 接收矩形左、下、右、上边界的指针。
返回值：成功时返回 TRUE。
10. FPDFPage_GetArtBox
获取页面字典中的 "ArtBox" 条目。
参数：page - 页面句柄，left、bottom、right、top - 接收矩形左、下、右、上边界的指针。
返回值：成功时返回 TRUE。
FPDFPage_TransFormWithClip
对页面应用变换。
参数：page - 页面句柄，matrix - 变换矩阵，clipRect - 裁剪矩形。
返回值：成功时返回 TRUE。
FPDFPageObj_TransformClipPath
变换页面对象的裁剪路径。
参数：page_object - 页面对象句柄，a、b、c、d、e、f - 矩阵系数。
13. FPDFPageObj_GetClipPath
获取页面对象的裁剪路径。
参数：page_object - 页面对象句柄。
返回值：裁剪路径的句柄，失败时返回 NULL。
14. FPDFClipPath_CountPaths
获取裁剪路径中的路径数量。
参数：clip_path - 裁剪路径句柄。
返回值：路径数量，失败时返回 -1。
15. FPDFClipPath_CountPathSegments
获取裁剪路径中某条路径的段数量。
参数：clip_path - 裁剪路径句柄，path_index - 路径索引。
返回值：段数量，失败时返回 -1。
FPDFClipPath_GetPathSegment
获取裁剪路径中某条路径的某个段。
参数：clip_path - 裁剪路径句柄，path_index - 路径索引，segment_index - 段索引。
返回值：段的句柄，失败时返回 NULL。
FPDF_CreateClipPath
创建一个新的裁剪路径，并插入一个矩形。
参数：left、bottom、right、top - 裁剪矩形的左、下、右、上边界。
返回值：裁剪路径的句柄。
FPDF_DestroyClipPath
销毁裁剪路径。
参数：clipPath - 裁剪路径句柄。
FPDFPage_InsertClipPath
裁剪页面内容，使页面内容在裁剪区域外不可见。
参数：page - 页面句柄，clipPath - 裁剪路径句柄。
这些函数提供了对 PDF 页面和页面对象的各种操作，包括设置和获取页面字典中的矩形条目、应用变换、处理裁剪路径等。
```

## 中文总结这里面的函数功能

```
以下是 include/fpdfview.h 文件中部分函数的功能总结：
FPDF_InitLibraryWithConfig
初始化 PDFium 库并分配全局资源。
参数：config - 配置信息。
返回值：无。
2. FPDF_InitLibrary
初始化 PDFium 库（替代形式）。
参数：无。
返回值：无。
3. FPDF_DestroyLibrary
释放由 FPDF_InitLibrary 或 FPDF_InitLibraryWithConfig 分配的全局资源。
参数：无。
返回值：无。
4. FPDF_SetSandBoxPolicy
设置沙盒环境的策略。
参数：policy - 指定的策略，enable - 启用或禁用策略。
返回值：无。
5. FPDF_LoadCustomDocument
从自定义访问描述符加载 PDF 文档。
参数：pFileAccess - 访问文件的结构，password - 解密 PDF 文件的可选密码。
返回值：加载的文档句柄，失败时返回 NULL。
6. FPDF_GetFileVersion
获取给定 PDF 文档的文件版本。
参数：doc - 文档句柄，fileVersion - PDF 文件版本。
返回值：成功时返回 TRUE。
FPDF_GetLastError
获取函数失败时的最后一个错误代码。
参数：无。
返回值：32 位整数，指示错误代码。
FPDF_DocumentHasValidCrossReferenceTable
检查文档的交叉引用表是否有效。
参数：document - 文档句柄。
返回值：交叉引用表有效时返回 TRUE。
9. FPDF_GetTrailerEnds
获取尾随结束的字节偏移量。
参数：document - 文档句柄，buffer - 接收字节偏移量的缓冲区，length - 缓冲区大小。
返回值：缓冲区中的整数数量，出错时返回 0。
FPDF_GetDocPermissions
获取文档的文件权限标志。
参数：document - 文档句柄。
返回值：32 位整数，指示权限标志。
11. FPDF_GetDocUserPermissions
获取文档的用户文件权限标志。
参数：document - 文档句柄。
返回值：32 位整数，指示权限标志。
FPDF_GetSecurityHandlerRevision
获取安全处理程序的修订版本。
参数：document - 文档句柄。
返回值：安全处理程序的修订版本号。
13. FPDF_GetPageCount
获取文档中的页面数量。
参数：document - 文档句柄。
返回值：页面数量。
FPDF_LoadPage
加载 PDF 文档中的页面。
参数：document - 文档句柄，page_index - 页面索引。
返回值：页面句柄，失败时返回 NULL。
15. FPDF_ClosePage
关闭已加载的 PDF 页面。
参数：page - 页面句柄。
返回值：无。
FPDF_CloseDocument
关闭已加载的 PDF 文档。
参数：document - 文档句柄。
返回值：无。
17. FPDF_DeviceToPage
将点的屏幕坐标转换为页面坐标。
参数：page - 页面句柄，start_x、start_y - 显示区域的左上角像素位置，size_x、size_y - 显示页面的水平和垂直尺寸，rotate - 页面方向，device_x、device_y - 设备坐标中的点，page_x、page_y - 接收转换后页面坐标的指针。
返回值：转换成功时返回 TRUE。
18. FPDF_RenderPageBitmapWithMatrix
使用矩阵渲染页面到位图。
参数：bitmap - 位图句柄，page - 页面句柄，matrix - 变换矩阵，clipping - 裁剪矩形，flags - 渲染标志。
返回值：无。
19. FPDF_VIEWERREF_GetPrintScaling
获取 PDF 文档是否希望缩放打印。
参数：document - 文档句柄。
返回值：是否缩放打印。
FPDF_VIEWERREF_GetNumCopies
获取打印的副本数量。
参数：document - 文档句柄。
返回值：打印的副本数量。
21. FPDF_VIEWERREF_GetPrintPageRange
获取打印时初始化打印对话框的页面范围。
参数：document - 文档句柄。
返回值：打印页面范围的句柄。
FPDF_VIEWERREF_GetPrintPageRangeCount
获取 FPDF_PAGERANGE 中的元素数量。
参数：pagerange - 页面范围句柄。
返回值：页面范围中的元素数量。
23. FPDF_VIEWERREF_GetPrintPageRangeElement
获取 FPDF_PAGERANGE 中的某个元素。
参数：pagerange - 页面范围句柄，index - 元素索引。
返回值：页面范围中指定索引的元素值。
24. FPDF_VIEWERREF_GetDuplex
获取打印对话框中使用的纸张处理选项。
参数：document - 文档句柄。
返回值：纸张处理选项。
25. FPDF_VIEWERREF_GetName
获取查看器引用的内容，给定键的值必须是 "name" 类型。
参数：document - 文档句柄，key - 查看器首选项字典中的键名（UTF-8 编码），buffer - 接收键值的缓冲区，length - 缓冲区长度。
返回值：内容的字节数，包括 NULL 终止符。
FPDF_CountNamedDests
获取 PDF 文档中命名目标的数量。
参数：document - 文档句柄。
返回值：命名目标的数量。
FPDF_GetNamedDestByName
通过名称获取命名目标。
参数：document - 文档句柄，name - 目标名称（UTF-8 编码）。
返回值：目标的句柄。
这些函数提供了对 PDF 文档的各种操作，包括初始化和销毁库、加载和关闭文档和页面、获取文档信息、处理错误、渲染页面、处理打印设置等。
```

## 中文总结这里面的函数功能

```
以下是 include/fpdf_formfill.h 文件中部分函数的功能总结：
表单处理相关函数
1. FORM_OnAfterLoadPage
在加载页面后调用此函数。
参数：page - 页面句柄，hHandle - 表单填充模块句柄。
返回值：无。
2. FORM_OnBeforeClosePage
在关闭页面前调用此函数。
参数：page - 页面句柄，hHandle - 表单填充模块句柄。
返回值：无。
3. FORM_DoDocumentJSAction
执行文档级别的 JavaScript 操作。
参数：hHandle - 表单填充模块句柄。
返回值：无。
4. FORM_DoDocumentOpenAction
执行文档打开时的操作。
参数：hHandle - 表单填充模块句柄。
返回值：无。
5. FORM_DoDocumentAAction
执行文档的附加操作。
参数：hHandle - 表单填充模块句柄，aaType - 附加操作类型。
返回值：无。
6. FORM_DoPageAAction
执行页面对象的附加操作。
参数：page - 页面句柄，hHandle - 表单填充模块句柄。
返回值：无。
鼠标和键盘事件处理函数
1. FORM_OnLButtonDoubleClick
处理鼠标左键双击事件。
参数：hHandle - 表单填充模块句柄，page - 页面句柄，modifier - 修饰键，page_x - 鼠标 x 坐标，page_y - 鼠标 y 坐标。
返回值：成功时返回 TRUE。
2. FORM_OnKeyDown
处理按键按下事件。
参数：hHandle - 表单填充模块句柄，page - 页面句柄，nKeyCode - 虚拟键码，modifier - 修饰键。
返回值：成功时返回 TRUE。
3. FORM_OnKeyUp
处理按键释放事件。
参数：hHandle - 表单填充模块句柄，page - 页面句柄，nKeyCode - 虚拟键码，modifier - 修饰键。
返回值：成功时返回 TRUE。
FORM_OnChar
处理字符输入事件。
参数：hHandle - 表单填充模块句柄，page - 页面句柄，nChar - 字符码，modifier - 修饰键。
返回值：成功时返回 TRUE。
表单填充信息结构体
1. FPDF_FORMFILLINFO
表单填充信息结构体，包含多个回调函数，用于处理表单相关的操作。
其他函数
1. FORM_GetFocusedText
获取当前聚焦字段中的文本。
参数：hHandle - 表单填充模块句柄，page - 页面句柄。
返回值：文本内容。
这些函数提供了对 PDF 表单的各种操作，包括加载和关闭页面、执行 JavaScript 操作、处理鼠标和键盘事件、获取聚焦字段的文本等。通过这些函数，可以实现对 PDF 表单的全面控制和操作。
```

