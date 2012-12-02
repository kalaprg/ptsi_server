var gl;
var canvas;
var program;

var viewX = 0.0;
var viewY = 0.0;

var viewStep = 1.0 / 256.0;

function initTextures() {
    graphTexture = gl.createTexture();
    graphImage = new Image();
    graphImage.onload = function () { handleTextureLoaded(cubeImage, cubeTexture); }
    graphImage.src = "test.png";
}

function handleTextureLoaded(image, texture) {
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_NEAREST);
    gl.generateMipmap(gl.TEXTURE_2D);
    gl.bindTexture(gl.TEXTURE_2D, null);
}


function InitWebGL() {

    //alert("InitWebGL");


    // Get A WebGL context
    canvas = document.getElementById("canvas1");
    gl = WebGLUtils.setupWebGL(canvas);
    
    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(0.0, 0.0, 0, 1);
    gl.clear(gl.COLOR_BUFFER_BIT);

    var v = document.getElementById("vertexshader").firstChild.nodeValue;
    var f = document.getElementById("fragmentshader").firstChild.nodeValue;

    var vs = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vs, v);
    gl.compileShader(vs);

    var fs = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fs, f);
    gl.compileShader(fs);

    program = gl.createProgram();
    gl.attachShader(program, vs);
    gl.attachShader(program, fs);
    gl.linkProgram(program);

    gl.useProgram(program);
    program.uColor = gl.getUniformLocation(program, "uColor");
    gl.uniform4fv(program.uColor, [1.0, 0.0, 0.0, 1.0]);

    var vbuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbuffer);

    if (!gl.getShaderParameter(vs, gl.COMPILE_STATUS))
        console.log(gl.getShaderInfoLog(vs));

    if (!gl.getShaderParameter(fs, gl.COMPILE_STATUS))
        console.log(gl.getShaderInfoLog(fs));

    if (!gl.getProgramParameter(program, gl.LINK_STATUS))
        console.log(gl.getProgramInfoLog(program));

    var aspect = canvas.width / canvas.height;

    
    

    Draw();

}

function Draw() {

    window.requestAnimFrame(Draw, canvas);


    var width = 256;
    var height = 256;
    var scale = 1;
    
    var vertices = new Float32Array(2 * 254);

    var size = 256.0;

    var vertexIndex = 0;
    var x = readIndexECG;
    for (var i = 0; i < width - 2; ++i) {

        vertices[vertexIndex] = ((i * 2.0) / size - 1.0);
        vertices[vertexIndex+1] =  ((dataECG[x] * 2.0) / size - 1.0);

        vertexIndex = vertexIndex + 2;

        x = UpdateIndex(x);
    }

    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

    var size = vertices.length / 2;

    program.aVertexPosition = gl.getAttribLocation(program, "aVertexPosition");
    gl.enableVertexAttribArray(program.aVertexPosition);
    gl.vertexAttribPointer(program.aVertexPosition, 2, gl.FLOAT, false, 0, 0);

    gl.drawArrays(gl.LINE_STRIP, 0, 254);
}

