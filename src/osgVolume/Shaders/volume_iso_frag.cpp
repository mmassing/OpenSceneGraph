char volume_iso_frag[] = "#version 110\n"
                         "\n"
                         "##pragma import_defines(NVIDIA_Corporation)\n"
                         "\n"
                         "#if defined(NVIDIA_Corporation)\n"
                         "    // workaround a NVidia hang when the loop variable is a float, but works fine when it's an int\n"
                         "    #define loop_type int\n"
                         "#else\n"
                         "    #define loop_type float\n"
                         "#endif\n"
                         "\n"
                         "uniform sampler3D baseTexture;\n"
                         "uniform float SampleDensityValue;\n"
                         "uniform float TransparencyValue;\n"
                         "uniform float IsoSurfaceValue;\n"
                         "\n"
                         "varying vec4 cameraPos;\n"
                         "varying vec4 vertexPos;\n"
                         "varying vec3 lightDirection;\n"
                         "varying mat4 texgen;\n"
                         "varying vec4 baseColor;\n"
                         "\n"
                         "void main(void)\n"
                         "{\n"
                         "    vec4 t0 = vertexPos;\n"
                         "    vec4 te = cameraPos;\n"
                         "\n"
                         "    if (te.x>=0.0 && te.x<=1.0 &&\n"
                         "        te.y>=0.0 && te.y<=1.0 &&\n"
                         "        te.z>=0.0 && te.z<=1.0)\n"
                         "    {\n"
                         "        // do nothing... te inside volume\n"
                         "    }\n"
                         "    else\n"
                         "    {\n"
                         "        if (te.x<0.0)\n"
                         "        {\n"
                         "            float r = -te.x / (t0.x-te.x);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "\n"
                         "        if (te.x>1.0)\n"
                         "        {\n"
                         "            float r = (1.0-te.x) / (t0.x-te.x);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "\n"
                         "        if (te.y<0.0)\n"
                         "        {\n"
                         "            float r = -te.y / (t0.y-te.y);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "\n"
                         "        if (te.y>1.0)\n"
                         "        {\n"
                         "            float r = (1.0-te.y) / (t0.y-te.y);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "\n"
                         "        if (te.z<0.0)\n"
                         "        {\n"
                         "            float r = -te.z / (t0.z-te.z);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "\n"
                         "        if (te.z>1.0)\n"
                         "        {\n"
                         "            float r = (1.0-te.z) / (t0.z-te.z);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "    }\n"
                         "\n"
                         "    t0 = t0 * texgen;\n"
                         "    te = te * texgen;\n"
                         "\n"
                         "    const loop_type max_iteratrions = loop_type(2048);\n"
                         "    loop_type num_iterations = loop_type(ceil(length((te-t0).xyz)/SampleDensityValue));\n"
                         "    if (num_iterations<loop_type(2)) num_iterations = loop_type(2);\n"
                         "    if (num_iterations>max_iteratrions) num_iterations = max_iteratrions;\n"
                         "\n"
                         "\n"
                         "    vec3 deltaTexCoord=(t0-te).xyz/float(num_iterations-loop_type(1));\n"
                         "    vec3 texcoord = te.xyz;\n"
                         "\n"
                         "    vec4 previousColor = texture3D( baseTexture, texcoord);\n"
                         "\n"
                         "    float normalSampleDistance = 1.0/512.0;\n"
                         "    vec3 deltaX = vec3(normalSampleDistance, 0.0, 0.0);\n"
                         "    vec3 deltaY = vec3(0.0, normalSampleDistance, 0.0);\n"
                         "    vec3 deltaZ = vec3(0.0, 0.0, normalSampleDistance);\n"
                         "\n"
                         "    while(num_iterations>loop_type(0))\n"
                         "    {\n"
                         "        vec4 color = texture3D( baseTexture, texcoord);\n"
                         "\n"
                         "        float m = (previousColor.a-IsoSurfaceValue) * (color.a-IsoSurfaceValue);\n"
                         "        if (m <= 0.0)\n"
                         "        {\n"
                         "            float r = (IsoSurfaceValue-color.a)/(previousColor.a-color.a);\n"
                         "            texcoord = texcoord - r*deltaTexCoord;\n"
                         "\n"
                         "            float a = color.a;\n"
                         "            float px = texture3D( baseTexture, texcoord + deltaX).a;\n"
                         "            float py = texture3D( baseTexture, texcoord + deltaY).a;\n"
                         "            float pz = texture3D( baseTexture, texcoord + deltaZ).a;\n"
                         "\n"
                         "            float nx = texture3D( baseTexture, texcoord - deltaX).a;\n"
                         "            float ny = texture3D( baseTexture, texcoord - deltaY).a;\n"
                         "            float nz = texture3D( baseTexture, texcoord - deltaZ).a;\n"
                         "\n"
                         "            vec3 grad = vec3(px-nx, py-ny, pz-nz);\n"
                         "            if (grad.x!=0.0 || grad.y!=0.0 || grad.z!=0.0)\n"
                         "            {\n"
                         "                vec3 normal = normalize(grad);\n"
                         "                float lightScale = 0.1 +  max(0.0, dot(normal.xyz, lightDirection))*0.9;\n"
                         "\n"
                         "                color.x = lightScale;\n"
                         "                color.y = lightScale;\n"
                         "                color.z = lightScale;\n"
                         "            }\n"
                         "\n"
                         "\n"
                         "            color.a = 1.0;\n"
                         "\n"
                         "            color *= baseColor;\n"
                         "\n"
                         "            gl_FragColor = color;\n"
                         "\n"
                         "            return;\n"
                         "        }\n"
                         "\n"
                         "        previousColor = color;\n"
                         "\n"
                         "        texcoord += deltaTexCoord;\n"
                         "\n"
                         "        --num_iterations;\n"
                         "    }\n"
                         "\n"
                         "    // we didn't find an intersection so just discard fragment\n"
                         "    discard;\n"
                         "}\n"
                         "\n";
