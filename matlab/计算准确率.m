

%% 算一个模型的准确率
[vertex, face] = read_mesh('C:\Users\zzc\Desktop\util\data\1.obj');
Areas = szy_GetAreaOfFaces_vf(vertex, face);         % 算面的面积
seg_origin = load('E:/3DModelData/PSB/Airplane/20.seg') + 1;            % 读取GT标签
test_seg = load('F:/zzc/MeshTransformer/result/psb_airplane/20.seg') + 1;   % 读取我的预测标签
successRatio = Get_SuccessRatio(test_seg,seg_origin,Areas');      % 参数：预测标签，gt，面积


% 批量化
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



