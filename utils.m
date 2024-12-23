%% 初始化
close all
clear
clc
%% 算一个模型的准确率
% 读取模型的顶点和面,obj和off都可以
[vertex, face] = read_mesh('C:\Users\zzc\Desktop\10.obj');
Areas = szy_GetAreaOfFaces_vf(vertex, face);
seg_origin = load('C:\Users\zzc\Desktop\10.txt') + 1;             % GT标签
test_seg = load('C:\Users\zzc\Desktop\10.seg') + 1;               % 预测标签
successRatio = Get_SuccessRatio(test_seg,seg_origin,Areas');      % 参数：预测标签，gt，面积


%% 可视化一个模型并保存
[vertex, face] = read_mesh('E:\Research\paper_code\STseg\data\coseg_aliens\test\10.obj');
mesh_label = load('E:\Research\draw\x\human\shrec_4\best\4.seg') + 1;      % 上色标签
figure();                                                  % 画布
szy_PlotMesh_Discrete_vf(vertex, face, mesh_label);        % 画图

% 保存
folder = 'E:\Research\draw\x\human\shrec_4\best\4';            % 写入文件夹
dlmwrite([char(folder), '.seg'], (mesh_label - 1)');           % 写入标签
% 写入obj和colorbar
szy_WriteMeshWithFaceColor_Discrete(vertex, face, [char(folder), '.seg.obj'], mesh_label+7);



% 最简化
[vertex, face] = read_mesh('E:\3DModelData\ShapeNetCore_Manifold\Guitar\2.obj');
mesh_label = load('E:\3DModelData\ShapeNetCore_Manifold\Guitar\2.seg') + 1;        % 上色标签
folder = 'C:\Users\zhenyu shu\Desktop\render\shapenetcore\2';                         % 写入文件夹
szy_WriteMeshWithFaceColor_Discrete(vertex, face, [char(folder), '.seg.obj'], mesh_label+7);

%% GC并保存
[vertex, face] = read_mesh('E:\Research\draw\x\lamps\lamps_19\best\19.obj');
mesh_label = load('E:\Research\draw\x\lamps\lamps_19\best\19.seg') + 1;        % 上色标签
pred = load('E:\Research\draw\x\lamps\lamps_19\best\19.prob');
segResult = szy_GraphCut_vf(vertex,face,0.3,pred',false);

% 算准确率
Areas = szy_GetAreaOfFaces_vf(vertex, face);                     % 算面积
seg_origin = load('E:\Research\draw\x\lamps\lamps_19\best\19.txt') + 1;   % 读取GT标签
successRatio = Get_SuccessRatio(segResult,seg_origin,Areas'); 



folder = 'E:\Research\draw\x\hand\hand_18\gc\gc';  % 写入文件夹
szy_WriteMeshWithFaceColor_Discrete(vertex, face, [char(folder), '.seg.obj'], mesh_label+7);


%% 
% 下采样
% 模型下采样

% 读入模型
% 1是原始模型，2是下采样后的模型
[vertex1, face1] = read_mesh('E:\Research\datasets\PSB\ant\raw\obj\1.obj');
[vertex2, face2] = perform_mesh_simplification(vertex1,face1,1500,'1');  % fileID
vertex2 = vertex2';
face2 = face2';

% 保存obj文件
filename = "E:\Research\datasets\PSB\ant\2000\obj\1.obj";
write_obj(filename, vertex2, face2);


% 把1的标签迁移到2上，2是我需要的1500个面的
% [vertex2, face2] = read_mesh('E:\Research\draw\datasets\COSEG\aliens\10.obj');
modelFileName1 = 'E:\Research\datasets\PSB\ant\raw\obj\1.obj';
seg1 = load('E:\Research\datasets\PSB\ant\face_label\1.seg')+1;
seg2 = szy_TransferSeg(modelFileName1, seg1, vertex2, face2);

% 保存seg2
folder = 'E:\Research\datasets\PSB\ant\2000\face_label\1.seg';
dlmwrite(folder, (seg2 - 1));  % 写入标签




% 批量化处理
close all
clear
clc
for i = 1:20
    datasets_name = 'COSEG';
    model_name = 'lamps';
    raw_off = ['E:\Research\datasets\', datasets_name,'\',model_name,'\raw\off\', int2str(i),'.off'];
    raw_seg = ['E:\Research\datasets\', datasets_name,'\',model_name,'\raw\face_label\', int2str(i), '.seg'];
    sub_obj = ['E:\Research\datasets\', datasets_name,'\',model_name,'\1500\obj\', int2str(i), '.obj'];
    sub_seg = ['E:\Research\datasets\', datasets_name,'\',model_name,'\1500\face_label\',int2str(i),'.txt'];

    [vertex1, face1] = read_mesh(raw_off);
    [vertex2, face2] = perform_mesh_simplification(vertex1,face1,1500, int2str(i));  % fileID
    vertex2 = vertex2';
    face2 = face2';
    write_obj(sub_obj, vertex2, face2);

    seg1 = load(raw_seg)+1;
    seg2 = szy_TransferSeg(raw_off, seg1, vertex2, face2);
    dlmwrite(sub_seg, (seg2 - 1));  % 写入标签
end



%% 上采样回去，插值，细分

% 主要是标签

[vertex2, face2] = read_mesh('E:\Research\datasets\PSB\ant\raw\obj\1.obj');
modelFileName1 = 'E:\Research\datasets\PSB\ant\1500\obj\1.obj';
seg1 = load('E:\Research\datasets\PSB\ant\1500\face_label\1.txt')+1;
seg2 = szy_TransferSeg(modelFileName1, seg1, vertex2, face2);


%% 格式之间的转换
% 把off变成obj
inMeshFileName = 'C:\Users\zzc\Desktop\1.off';
outMeshFileName = 'C:\Users\zzc\Desktop\1.obj';
szy_ConvertModelFormat(inMeshFileName, outMeshFileName)



% 批量化转换
for i = 1:20
    inMeshFileName = ['E:\Research\paper_code\STseg\data\psb_teddy\model\', int2str(i),'.off'];
    outMeshFileName = ['E:\Research\paper_code\STseg\data\psb_teddy\model\', int2str(i),'.obj'];
    szy_ConvertModelFormat(inMeshFileName, outMeshFileName)
end





%%
% 临时















%% 算所有结果的准确率
successRatio = {};
for i = 1:20
    offName = ['E:/3DModelData/PSB/Teddy/', int2str(i), '.off'];
    segName = ['E:/3DModelData/PSB/Teddy/', int2str(i), '.seg'];
    [vertex, face] = read_mesh(offName);
    Areas = szy_GetAreaOfFaces_vf(vertex, face);
    seg_origin = load(segName) + 1;
    test_seg_name = ['./PLable/teddy2/', int2str(i-1),'.seg'];
    test_seg = load(test_seg_name) + 1;
    successRatio{i} = Get_SuccessRatio(test_seg,seg_origin,Areas');
end


%% 可视化并保存
[vertex, face] = read_mesh('E:/3DModelData/PSB/Teddy/19.off');  % 读入模型
res = computeTriangleBarycentric(vertex, face);                 % 计算三角形重心
% 用重心的顺序找到最近的面的顺序索引
mesh_ids = szy_FindClosestFaceByPoint('E:/3DModelData/PSB/Teddy/19.off', res);   
seg_origin = load('E:/3DModelData/PSB/Teddy/19.seg') + 1;      % 读入上色标签
mesh_label = seg_origin(mesh_ids);                    % 获得对应面的标签
figure();                                             % 画布
szy_PlotMesh_Discrete_vf(vertex, face, mesh_label);   % 画图

% 保存
folder = './zzcTEST./19';  % 写入文件夹
dlmwrite([char(folder), '.seg'], (mesh_label - 1)');  %写入标签
% 写入obj和colorbar
szy_WriteMeshWithFaceColor_Discrete(vertex, face, [char(folder), '.seg.obj'], mesh_label-1);








%% 图割+算准确率+可视化
% 图割
[vertex, face] = read_mesh('E:\Research\paper_code\STseg\data\coseg_aliens\test\10.obj');
pred = load('E:\Research\paper_code\STseg\runs\coseg_aliens_20240814_161059\result_best\10.prob');
segResult = szy_GraphCut_vf(vertex,face,0.3,pred',false);
% 算准确率
Areas = szy_GetAreaOfFaces_vf(vertex, face);                % 算面积
seg_origin = load('E:\Research\paper_code\STseg\data\coseg_aliens\face_label\10.txt') + 1;   % 读取GT标签
successRatio = Get_SuccessRatio(segResult,seg_origin,Areas'); 


% 可视化
res = computeTriangleBarycentric(vertex, face);
mesh_ids = szy_FindClosestFaceByPoint('E:/3DModelData/PSB/Teddy/19.off', res);
mesh_label = segResult(mesh_ids);
figure();                                             
szy_PlotMesh_Discrete_vf(vertex, face, mesh_label);
% 保存
folder = 'C:\Users\zzc\Desktop\STseg\render\coseg_aliens\10\3';  % 写入文件夹
% dlmwrite([char(folder), '.seg'], (mesh_label - 1)');  %写入标签
% 写入obj和colorbar
szy_WriteMeshWithFaceColor_Discrete(vertex, face, [char(folder), '.seg.obj'], mesh_label+7);








