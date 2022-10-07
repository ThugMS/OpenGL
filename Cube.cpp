//#include <vgl.h>
//#include <initshader.h>
//#include <vec.h>
//
//const int num_points = 36;
//
//vec4 points[num_points];
//vec4 colors[num_points];
//
//vec4 vertex_pos[8] = {
//	vec4(-0.5, -0.5, 0.5, 1.0),
//	vec4(-0.5, 0.5, 0.5, 1.0),
//	vec4(0.5, 0.5, 0.5, 1.0),
//	vec4(0.5, -0.5, 0.5, 1.0),
//
//	vec4(-0.5, -0.5, -0.5, 1.0),
//	vec4(-0.5, -0.5, -0.5, 1.0),
//	vec4(0.5, 0.5, -0.5, 1.0),
//	vec4(0.5, -0.5, -0.5, 1.0)
//};
//
//vec4 vertex_color[8] = {
//	vec4(1,1,1,1),
//	vec4(1,0,0,1),
//	vec4(0,1,0,1),
//	vec4(0,0,1,1),
//	vec4(1,1,0,1),
//	vec4(1,0,1,1),
//	vec4(0,1,1,1),
//	vec4(0,0,0,1)
//};
//
//void setrectangle(int a, int b, int c, int d) {
//
//	static int index = 0;
//
//	points[index] = vertex_pos[a];	colors[index] = vertex_color[a];  index++;
//	points[index] = vertex_pos[b];	colors[index] = vertex_color[b];  index++;
//	points[index] = vertex_pos[c];	colors[index] = vertex_color[c];  index++;
//
//	points[index] = vertex_pos[c];	colors[index] = vertex_color[c];  index++;
//	points[index] = vertex_pos[d];	colors[index] = vertex_color[d];  index++;
//	points[index] = vertex_pos[a];	colors[index] = vertex_color[a];  index++;
//}
//
//void init() {
//	// 1. create data in cpu
//
//	//반시계 방향으로 하는게 국룰이다.
//	setrectangle(0, 3, 2, 1);
//	setrectangle(4, 5, 6, 7);
//	setrectangle(0, 4, 7, 3);
//	setrectangle(3, 7, 6, 2);
//	setrectangle(2, 6, 5, 1);
//	setrectangle(0, 1, 5, 4);
//
//	// 2. sending the data to gpu
//	// 2-1. create vertex array object
//	gluint vao;
//	glgenvertexarrays(1, &vao);
//	glbindvertexarray(vao);
//
//	// 2-2. create vertex buffer object
//	gluint vbo;
//	glgenbuffers(1, &vbo);
//	glbindbuffer(gl_array_buffer, vbo);
//
//	// 2-3. sending the data to the buffer
//	glbufferdata(gl_array_buffer, sizeof(points) + sizeof(colors), null, gl_static_draw); //null 인 이유는 아직 데이터의 공간만큼만 확보하고 데이터를 넘기지는 않음
//	glbuffersubdata(gl_array_buffer, 0, sizeof(points), points); // 이제부터 확보해놓은 공간에 데이터를 넘기는 것
//	glbuffersubdata(gl_array_buffer, sizeof(points), sizeof(colors), colors);
//
//	// 3. loading the shaders
//	gluint prog = initshader("vshader_cube.glsl", "fshader_cube.glsl");
//	gluseprogram(prog);
//
//	// 4. connect the data with the shaders
//	gluint vposition = glgetattriblocation(prog, "vposition");
//	gldisablevertexattribarray(vposition);
//	glvertexattribpointer(vposition, 4, gl_float, false, 0, buffer_offset(0));
//
//	gluint vcolor = glgetattriblocation(prog, "vcolor");
//	gldisablevertexattribarray(vcolor);
//	glvertexattribpointer(vcolor, 4, gl_float, false, 0, buffer_offset(sizeof(points)));
//}
//
//void display() {
//	glclear(gl_color_buffer_bit);
//	gldrawarrays(gl_triangles, 0, num_points);
//	glflush();
//}
//
//int main(int argc, char** argv)
//{
//	glutinit(&argc, argv);
//	glutinitdisplaymode(glut_single | glut_rgba);
//	glutinitwindowsize(512, 512);
//	glutcreatewindow("hello gl");
//
//	glewexperimental = true;
//	glewinit();
//	init();
//	glutdisplayfunc(display);
//	glutmainloop();
//
//	return 0;
//}