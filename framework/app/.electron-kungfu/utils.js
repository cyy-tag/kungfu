const fs = require("fs");
const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');

const rootDir = path.dirname(__dirname);

const getCommitVersion = () => {
  const gitDir = path.resolve(rootDir, '..', '..', '.git')
  var gitCommitVersion = 'latest'
  try {
    var gitHEAD = fs.readFileSync(path.join(gitDir, 'HEAD'), 'utf-8').trim() // ref: refs/heads/develop
    if((gitHEAD || '').split(': ').length <= 1) {
      gitCommitVersion = gitHEAD
    } else {
      var ref = gitHEAD.split(': ')[1] // refs/heads/develop
      var develop = gitHEAD.split('/')[2] // 环境：develop
      var gitVersion = fs.readFileSync(path.join(gitDir, ref), 'utf-8').trim() // git版本号，例如：6ceb0ab5059d01fd444cf4e78467cc2dd1184a66
      gitCommitVersion = develop + ': ' + gitVersion
    }
  } catch (err) {
    console.error(err)
  }
  return gitCommitVersion.toString();
}


const getPythonVersion = () => {
  var pyVersion = "3";
  try {
    var buildInfoRaw = fs.readFileSync(path.join(path.dirname(rootDir), 'core', 'dist', 'kfc', 'kungfubuildinfo.json'), 'utf-8')
    var buildInfo = JSON.parse(buildInfoRaw);
    pyVersion = buildInfo.pythonVersion || "3"
  } catch (err) {
    console.error(err)
  }
  return pyVersion
}

const listDirSync = (filePath) => {
  return fs.readdirSync(filePath)
}

const getViewsConfig = () => {
  const viewsPath = path.resolve(__dirname, "..", "src", "renderer", "views");
  const files = listDirSync(viewsPath);
  let entry = {}, plugins = [];
  files
  .filter(file => {
    const filePath = path.join(viewsPath, file);
    const stats = fs.statSync(filePath);
    return stats.isDirectory();
  })
  .forEach(file => {
    entry[file] = path.join(rootDir, 'src', 'renderer', 'views', file, 'main.js'),
    plugins.push( new HtmlWebpackPlugin({
      filename: `${file}.html`,
      template: path.resolve(rootDir, 'src', 'index.ejs'),
      minify: {
        collapseWhitespace: true,
        removeAttributeQuotes: true,
        removeComments: true
      },
      chunks: [file],
      nodeModules: process.env.NODE_ENV !== 'production'
        ? path.resolve(rootDir, 'node_modules')
        : false
    }))
  });

  return {
    entry,
    plugins
  }
}

exports.getCommitVersion = getCommitVersion;
exports.getPythonVersion = getPythonVersion;
exports.getViewsConfig = getViewsConfig;