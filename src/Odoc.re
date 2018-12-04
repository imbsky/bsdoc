open Common;
open Logger;

module Bin = {
  let odoc = run("odoc");

  let support_files = out_dir =>
    odoc(["support-files", "--output-dir", out_dir]);

  let compile = (lib_dir, pkg_name, file_name) =>
    odoc(["compile", "-I", lib_dir, {j|--pkg=$pkg_name|j}, file_name]);

  let compile_mld = (lib_dir, out_file, pkg_name, file_name) =>
    odoc([
      "compile",
      "-I",
      lib_dir,
      {j|--pkg=$pkg_name|j},
      "-o",
      out_file,
      file_name,
    ]);
  let html = (lib_dir, out_dir, file_name) =>
    odoc([
      "html",
      "-I",
      lib_dir,
      "-o",
      out_dir,
      "--syntax=re",
      "--semantic-uris",
      file_name,
    ]);
};

let support_files = Bin.support_files;

let filename = path => Filename.(path |> basename |> chop_extension);

let as_odoc = name => filename(name) ++ ".odoc";

let cmti_to_odoc = (lib_dir, pkg_name, cmti) => {
  let odoc = as_odoc(cmti);
  debug({j|Compiling $cmti to $odoc...|j});
  Bin.compile(lib_dir, pkg_name, Filename.concat(lib_dir, cmti));
  odoc;
};

let mld_to_odoc = (lib_dir, src_dir, pkg_name, mld) => {
  let odoc = as_odoc(mld);
  debug({j|Compiling $mld to $odoc...|j});
  Bin.compile_mld(
    lib_dir,
    Filename.concat(lib_dir, mld),
    pkg_name,
    Filename.concat(src_dir, mld),
  );
  odoc;
};

let odoc_to_html = (lib_dir, out_dir, odoc) => {
  let html = filename(odoc) ++ ".html";
  debug({j|Compiling $odoc to $html...|j});
  Bin.html(lib_dir, out_dir, Filename.concat(lib_dir, odoc));
  html;
};
