<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN">
<html>
<meta http-equiv="Content-Type" content="text/html;charset=utf-16" />
<head>
	<title>VNOC</title>
</head>
<body>
	<style>
		p.p1{color:red;font-size:17pt;font-family:黑体;font-weight:bold;}
		p.p2{color:blue;font-size:17pt;font-family:楷体GB_2312;font-weight:bold;}
		div.wrap{margin:0 auto;background-color:gray;border:blue dashed thin;overflow:auto; overflow-x:hidden;text-align:center;}
		div.wrap span{display:inline-block;height:30px;width:19%;background-color:orange;border:green dotted thin;text-align:center;}
	</style>
	<center>
	<p class="p1">Hello，这是VNOC网页版。</p>
	<p class="p1">这是VNOC主页。</p>
	<p class="p1">但是，它还在建设中。</p>
	<p></p>
	<p class="p2">如果，你愿意辅助我们进行浏览器的兼容性测试，</p>
	<p class="p2">请点击下面的任何一个按钮，</p>
	<p class="p2">你要求填写一个表单，以便我们完成测试。</p>
	<p class="p2">每个测试大概只需要你3-4分钟时间，谢谢你的帮助！</p>
	<div class="wrap">
		<?php
			require_once("less_test/sections.php");
			foreach($section as $id => $value)
				echo "<span><a href='less_test/form.php?section={$id}'>{$value}</a></span>";
		?>
	</div>
	</center>
</body>
</html>