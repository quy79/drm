<?
//header("Connection: Keep-Alive");
//header("Keep-Alive: timeout=3000"); 

//phpinfo();
//require_once( "aes.php");

if ($_POST["action"] == "upload"){

//create random directory name
$dir_name = uniqid();

//echo $_POST["upnames"];
$upnames = explode('|', $_POST["upnames"]);
$zipdir = "upload";
if (!is_dir($zipdir))
	mkdir($zipdir);
$zipname = $zipdir . '/'. $dir_name . '.zip';
//$zip = new ZipArchive();
//if ($zip->open($zipname, ZIPARCHIVE::CREATE)!==TRUE) {
//	exit("cannot open <$zipname>\n");
//}

foreach($upnames as $upname){

	if (!is_dir($zipdir.'/'.$dir_name))
		mkdir($zipdir.'/'.$dir_name);	
	
	$filename = basename( $_FILES[$upname]['name']);
	$filter_filename = preg_replace('/[[:blank:]]+/', '_', $filename);
	$drmname = $filename . ".drm";
	$zip_drmname = $zipdir . '/'. uniqid() . '_' . $drmname;
	$temppath = $_FILES[$upname]['tmp_name'];
	move_uploaded_file($temppath, '/home/quy/httpd/html/upload/'.$dir_name.'/'.$filter_filename);
	
	/*
	$handle = fopen($temppath, "rb");
	$contents = fread($handle, filesize($temppath));
	fclose($handle);
	try{
		$enc_contents = encrypt_AES($contents);
	}catch(Exception $e){
		echo $e->getMessage();
	}
	$zip2 = new ZipArchive();
	if ($zip2->open($zip_drmname, ZIPARCHIVE::CREATE) == true)
	{
		$zip2->addFromString($filename, $enc_contents);
		$zip2->close();

		$h = fopen($zip_drmname, "rb");
		$zip->addFromString($drmname, fread($h, filesize($zip_drmname)));
		fclose($h);
		//$zip->addFile(realpath($zip_drmname), $drmname);
		unlink($zip_drmname);
	}
	*/
	unlink($temppath);
	
}

//execute the DRM app
$pgpath = '/home/quy/workspace/drm/dev/drm';
$cmd = $pgpath . ' ' . '/home/quy/httpd/html/upload/' . $dir_name;
//$cmd = $pgpath . ' ' . $original_file . ' ' . $drm_filename . ' ' . $zip_filename;
$last_line =  exec($cmd, $file_list, $return_code);

	
//echo "numfiles: " . $zip->numFiles . "\n";
//echo "status:" . $zip->status . "\n";
#if (file_exists('readme.txt'))
#	$zip->addFile('readme.txt',mb_convert_encoding('ファイルの開き方.txt', 'SJIS'));
#if (file_exists('Setup.exe'))
#	$zip->addFile('Setup.exe');
/*
$text = "ファイルの開き方";
$zip->addFromString(mb_convert_encoding('ファイルの開き方.txt', 'SJIS'), $text);

$zip->close();
*/
$ret_filename = substr($last_line, strrpos($last_line, "/")+1);
$ret_filepath =  'upload/' . substr($last_line, strrpos($last_line, "/")+1);


echo "The files have been successfully uploaded, please visit the following link to download:<br /> <a href='" . $ret_filepath . "'>" . $ret_filename . "</a>";
}
?>

<script type="text/javascript">
function onSubmit(){
	var fns = [];
	for (var i = 0; i < g_maxIndex; ++i){
		var ofile = document.getElementById('DRM_'+i);
		if(!ofile) continue;

		if(ofile.value.length > 0)
			fns.push('DRM_'+i);
	}

	if (fns.length == 0){
		alert('Please choose at least one file to upload!');
		return false;
	}

	document.getElementById('upnames').value = fns.join('|');

	return true;
}

var g_maxIndex = 1;
function onAddNew(){
	var container = document.getElementById('files_container');
	var oInput = document.createElement('input');
	oInput.type='file';
	oInput.id = oInput.name='DRM_' + (g_maxIndex++);
	oInput.className = 'file';
	container.appendChild(oInput);
}


</script>
<style type="text/css">
.file {
	clear:both;
	display:block;

	margin:5px;
}
</style>

<form enctype="multipart/form-data" action="upload.php" method="POST" onsubmit="return onSubmit()">

<div id="files_container">
  <input class="file" type="file" id="DRM_0" name="DRM_0" />
</div>
<input type="hidden" id="upnames" name="upnames" value="" />
<input type="hidden" name="action" value="upload" />
<br /><br />
<input type="button" value="Add new" onclick="onAddNew()" />
<input type="submit" value="upload" />
</form>
