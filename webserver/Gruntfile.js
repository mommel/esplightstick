const sass = require('node-sass');

module.exports = function(grunt) {
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    "uncss": { 
      "build": { 
        "files": { 
          "build/els.min.css": [
            "dev/config.html",
            "dev/index.html"
          ],
          "build/elsdz.min.css": [
            "dev/images.html"
          ] 
        } 
      }
    },
    "uglify": {
      "els": {
        files: {
          'build/els.min.js': [
            'node_modules/bootstrap/dist/js/bootstrap.js',
            'node_modules/jquery/dist/jquery.js',
            'src/js/esplightstick.js',
            'build/temp.images.js'
          ]
        },
        options: {
          mangle: false,
          "compress": true  
        }
      },
      "elsdz": {
        files: {
          'build/elsdz.min.js': [
            'node_modules/dropzone/dist/dropzone.js',
            'node_modules/popper.js/dist/popper.js'
          ]
        },
        options: {
          mangle: false,
          "compress": true  
        }
      }
    },
    "copy": {
      "build": { 
        "files": [ 
          { 
            "cwd": "node_modules/@fortawesome/fontawesome-free/css", 
            "src": [
              'all.css'
            ], 
            "dest": "build/", 
            "expand": true 
          },
          { 
            "cwd": "node_modules/@fortawesome/fontawesome-free/webfonts", 
            "src": [
              '*'
            ], 
            "dest": "build/", 
            "expand": true 
          },
          { 
            "cwd": "src/assets", 
            "src": [
              '*'
            ], 
            "dest": "build/", 
            "expand": true 
          }
        ] 
      },
      "build-uncss": { 
        "files": [ 
          { 
            "cwd": "node_modules/@fortawesome/fontawesome-free/webfonts", 
            "src": [
              '*'
            ], 
            "dest": "build/", 
            "expand": true 
          },
          { 
            "cwd": "src/assets", 
            "src": [
              '*'
            ], 
            "dest": "build/", 
            "expand": true 
          }
        ] 
      },
      "dev": { 
        "files": [ 
          { 
            "cwd": "node_modules/@fortawesome/fontawesome-free/webfonts", 
            "src": [
              '*'
            ], 
            "dest": "dev/", 
            "expand": true 
          },
          { 
            "cwd": "node_modules/@fortawesome/fontawesome-free/css", 
            "src": [
              'all.css'
            ], 
            "dest": "dev/", 
            "expand": true 
          },
          { 
            "cwd": "sd-card/images", 
            "src": [
              '*.bmp'
            ], 
            "dest": "dev/images", 
            "expand": true 
          },
          { 
            "cwd": "node_modules/dropzone/dist", 
            "src": [
              'dropzone.css',
              'dropzone.js'
            ], 
            "dest": "dev/", 
            "expand": true 
          },
          { 
            "cwd": "node_modules/bootstrap/dist/css", 
            "src": [
              'bootstrap.css'
            ], 
            "dest": "dev/", 
            "expand": true 
          },
          { 
            "cwd": "src/css", 
            "src": [
              '*.css'
            ], 
            "dest": "dev/", 
            "expand": true 
          },
          { 
            "cwd": "src/js", 
            "src": [
              '*.js'
            ], 
            "dest": "dev/", 
            "expand": true 
          },
          { 
            "cwd": "node_modules/bootstrap/dist/js", 
            "src": [
              'bootstrap.js',
              'bootstrap.js.map'
            ], 
            "dest": "dev/", 
            "expand": true 
          },
          { 
            "cwd": "node_modules/jquery/dist",           
            "src": [
              'jquery.js'
            ], 
            "dest": "dev/", 
            "expand": true 
          },
          { 
            "cwd": "node_modules/popper.js/dist/",           
            "src": [
              'popper.js'
            ], 
            "dest": "dev/", 
            "expand": true 
          },
          { 
            "cwd": "src/assets", 
            "src": [
              '*'
            ], 
            "dest": "dev/", 
            "expand": true 
          }
        ] 
      }
    },
    "cssmin": {
      "build": {
        "files": {
          "build/els.min.css": [
            "node_modules/bootstrap/dist/css/bootstrap.css",
            "src/css/esplightstick.css"
          ],
          "build/elsdz.min.css": [
            "build/temp.fontawesome.css",
            "node_modules/dropzone/dist/dropzone.css"
          ]
        }               
      } 
    },
    env : {
      options : {
    
      },
      dev : {
        NODE_ENV : 'development',
        DEST     : 'dev'
      },
      build : {
        NODE_ENV : 'production',
        DEST     : 'build'
      }
    },
    "prettify": {
      "options": {
        "indent": 2,
        "indent_char": ' ',
        "wrap_line_length": 78,
        "brace_style": 'expand',
        "unformatted": ['a', 'sub', 'sup', 'b', 'i', 'u']
      },
      "files": {
        'src/html/config.html': ['src/html/pretty.config.html'],
        'src/html/images.html': ['src/html/pretty.images.html'],
        'src/html/index.html': ['src/html/pretty.index.html']
      }
    },
    "zopfli": {
      "build": {
        files: [{
          expand: true,
          cwd: 'build/',
          src: ['*.{html,png,jpg,js,css,eot,svg,ttf,woff,woff2}'],
          dest: 'build/'
        }],
        options: {
          mode: 'gzip',
          extension: '.gz',
          limit: 1
        }
      }
    },
    "htmlmin": {
      "build": {
        "options": {
          "removeComments": true,
          "collapseWhitespace": true
        },
        "files": {
          'build/config.html': 'build/temp.config.html',
          'build/images.html': 'build/temp.images.html',
          'build/index.html': 'build/temp.index.html',
          'build/license.html': 'build/temp.license.html'
        }
      }
    },
    "processhtml": {
      options: {
        recursive: true,
        strip: true,
        includeBase: 'src/html'
      },
      "dev": {
        "files": {
          'dev/config.html': ['src/html/config.html'],
          'dev/images.html': ['src/html/images.html'],
          'dev/index.html': ['src/html/index.html'],
          'dev/license.html': ['src/html/license.html'],
        }
      },
      "build": {
        "files": {
          'build/temp.config.html': ['src/html/config.html'],
          'build/temp.images.html': ['src/html/images.html'],
          'build/temp.index.html': ['src/html/index.html'],
          'build/temp.license.html': ['src/html/license.html'],
        }
      }
    },
    'string-replace': {
      "build": {
        "files": [
          {
            "src": 'build/all.css',
            "dest": 'build/temp.fontawesome.css'
          },
        ],
        "options": {
          "replacements": [{
            "pattern": '/(../webfonts)/ig/',
            "replacement": '/'
          }]
        }
      },      
      "dev": {
        "files": [
          {
            "src": 'dev/all.css',
            "dest": 'dev/fontawesome.css'
          },
        ],
        "options": {
          "replacements": [{
            "pattern": '../webfonts/',
            "replacement": '/'
          }]
        }
      },
      "build_js": {
        "files": [
          {
            "src": 'src/js/images.js',
            "dest": 'build/temp.images.js'
          },
        ],
        "options": {
          "replacements": [{
            "pattern": 'http://localhost:3000',
            "replacement": 'http://192.168.1.1'
          }]
        }
      }
    },
    "clean": {
      prebuild: {
        src: ['build']
      },
      predev: {
        src: ['dev']
      },
      build: {
        src: ['build/temp.*', 'build/all.css']
      },
      dev: {
        src: ['dev/all.css']
      }
    },
    "autoprefixer" : {
      dev:{
        files:{
          'dev/*.css':'dev/*.css'
        }
      },
      build:{
        files:{
          'build/*.css':'build/*.css'
        }
      },
    },
    "sass": {
      "options": {
        implementation: sass,
        sourceMap: true
      },
      "dev": {
        files: [{
          expand: true,
          cwd: "src/sass/",
          src: ["*.scss"],
          dest: "dev/",
          ext: ".css"
        }]
      },
      "build": {
        options:{
          style:'compressed'
        },
        files: [{
          expand: true,
          cwd: "src/sass/",
          src: ["*.scss"],
          dest: "build/",
          ext: ".css"
        }]
      }
    },
    "watch": {
      options: {
        livereload: true
      },
      "express": {
        files:  [ 'dev/*.*' ],
        tasks:  [ 'express:server' ],
        options: {
          spawn: false
        }
      },
      "html": {
        "files": 'src/html/**.*',
        "tasks": ['processhtml:dev']
      },
      "scripts": {
        "files": 'src/sass/*.scss',
        "tasks": ['sass:dev', 'autoprefixer:dev']
      }
    },
    "express": {
      server: {
        options: {
          hostname: '*',
          script: './dev.app.js',
          livereload: {
            port: 35729,
            watch: ['src/html/**/*', 'src/css', 'src/js', 'src/sass']
          },
          logger: true,
        }
      }
    }
  });

  grunt.loadNpmTasks('grunt-autoprefixer');
  grunt.loadNpmTasks('grunt-contrib-clean');
  grunt.loadNpmTasks('grunt-contrib-concat');
  grunt.loadNpmTasks('grunt-contrib-copy');
  grunt.loadNpmTasks('grunt-contrib-cssmin');
  grunt.loadNpmTasks('grunt-contrib-htmlmin');  
  grunt.loadNpmTasks('grunt-contrib-uglify-es');
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-env');
  grunt.loadNpmTasks('grunt-express');
  grunt.loadNpmTasks('grunt-express-server');
  grunt.loadNpmTasks('grunt-prettify');
  grunt.loadNpmTasks('grunt-processhtml');
  grunt.loadNpmTasks('grunt-sass');
  grunt.loadNpmTasks('grunt-string-replace');
  grunt.loadNpmTasks('grunt-uncss');
  grunt.loadNpmTasks('grunt-zopfli-native');

  grunt.registerTask('dev', ['clean:predev', 'sass:dev', 'autoprefixer:dev', 'copy:dev', 'processhtml:dev', 'string-replace:dev', 'clean:dev', 'express', 'watch' ]); 
  grunt.registerTask('devwatch', ['dev', 'express', 'watch' ]);
  grunt.registerTask('build', ['clean:prebuild',  'sass:build', 'autoprefixer:build', 'copy:build', 'string-replace:build', 'uglify', 'cssmin:build', 'processhtml:build', 'htmlmin:build', 'clean:build', "zopfli:build"]);
  grunt.registerTask('default', ['dev']);
  grunt.registerTask('build-uncss', ['clean:prebuild', 'copy:build-uncss', 'string-replace:build', 'dev', 'uncss:build', 'processhtml:build', 'htmlmin:build', 'clean:build', "zopfli:build"]);
};
