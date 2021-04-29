# ### 第1步：导入库

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import MinMaxScaler
from keras.models import Sequential
from keras.layers import Dense, LSTM, Dropout, GRU
from keras.optimizers import SGD

# ### 第2步：加载历史数据文件

dataset = pd.read_csv("datasets_8388_11883_IBM_2006-01-01_to_2018-01-01.csv", 
                       index_col='Date', parse_dates=['Date'])

dataset.head()

# 检查数据是否有缺失值
train_set = dataset[:'2016'].iloc[:, 1:2].values # 训练集，获取到第一列的数据
test_set = dataset['2017':].iloc[:,1:2].values # 测试集，获取到第一列的数据

# ### 第3步：定义显示函数

def plot_predictions(test_result, predict_restult):
    """
    test_result: 真实值
    predict_result: 预测值
    """
    plt.plot(test_result, color='red', label='IBM True Stock Price')
    plt.plot(predict_restult, color='blue', label="IMB prdicted Stock Price")
    plt.title("IBM Stock Price")
    plt.xlabel("Time")
    plt.ylabel("Stock Price")
    plt.legend() # 给图加上图例
    plt.show()

# ### 第4步：绘制训练集和测试集的数据

dataset['High'][:"2016"].plot(figsize=(16, 4), legend=True)
dataset['High']["2017":].plot(figsize=(16, 4), legend=True)
plt.title("IBM Stock Price")
plt.legend(['Train set(before 2016)', 'Test set(after 2017)'])
plt.show()

# 正则化：将每一维的特征映射到指定的区间：【0，1】
sc = MinMaxScaler(feature_range=[0, 1])
train_set_scaled = sc.fit_transform(train_set)

# 创建序列数据集（训练和测试）
# 60个时间步为一个样本，1个输出
X_train = []
y_train = []
for i in range(60, 2769):
    X_train.append(train_set_scaled[i-60:i, 0]) 
    y_train.append(train_set_scaled[i, 0])

X_train, y_train = np.array(X_train), np.array(y_train) # numpy类型变换

X_train[0]

# LSTM的输入：(samples, sequence_length, features) 
# reshape: 训练集(2709,60)  ---> (2709, 60, 1)
X_train = np.reshape(X_train, (X_train.shape[0], X_train.shape[1], 1)) 
# ### 搭建LSTM模型，进行训练和预测

model = Sequential()
# LSTM 第一层
model.add(LSTM(128, return_sequences=True, input_shape=(X_train.shape[1], 1)))
model.add(Dropout(0.2))

# LSTM 第二层
model.add(LSTM(128, return_sequences=True))
model.add(Dropout(0.2))

# LSTM 第三层
model.add(LSTM(128))
model.add(Dropout(0.2))

# Dense层
model.add(Dense(units=1))

# 模型编译
model.compile(optimizer='rmsprop', loss='mse')

# 模型训练
model.fit(X_train, y_train, epochs=20, batch_size=32)


# ### 构建数据集，进行预测

dataset_total = pd.concat((dataset['High'][:"2016"], dataset['High']["2017":]), axis=0)
inputs = dataset_total[len(train_set):].values
inputs = inputs.reshape(-1, 1)
inputs_scaled = sc.fit_transform(inputs)

# ### 构建测试集X_test, 进行估价预测

dataset_total = pd.concat((dataset['High'][:"2016"], dataset['High']["2017":]), axis=0)
# 获取输入数据
inputs = dataset_total[len(dataset_total) - len(test_set) - 60:].values

# 归一化
inputs = inputs.reshape(-1, 1)
inputs = sc.transform(inputs)

# 准备测试集X_test,进行股价预测
X_test = []
for i in range(60, 311):
    X_test.append(inputs[i-60:i, 0])
    
X_test = np.array(X_test) # numpy 类型转换
X_test = np.reshape(X_test, (X_test.shape[0], X_test.shape[1], 1))
predict_test = model.predict(X_test) # 预测
predict_stock_price = sc.inverse_transform(predict_test)

# 绘制测试结果和预测结果
plot_predictions(test_set, predict_stock_price)


'''
# ### 基于GRU实现股价预测

model_gru = Sequential()
model_gru.add(GRU(50, return_sequences=True, input_shape=(X_train.shape[1], 1), activation='tanh'))
model_gru.add(Dropout(0.2))
model_gru.add(GRU(50, activation='tanh'))
model_gru.add(Dropout(0.2))
model_gru.add(Dense(1))

# 模型编译
model_gru.compile(optimizer=SGD(lr=0.01, decay=1e-7, momentum=0.9), loss='mse')

# 训练
model_gru.fit(X_train, y_train, epochs=20, batch_size=32)

# 准备测试集X_test,进行股价预测
X_test = []
for i in range(60, 311):
    X_test.append(inputs[i-60:i, 0])
    
X_test = np.array(X_test) # numpy 类型转换

X_test = np.reshape(X_test, (X_test.shape[0], X_test.shape[1], 1))

GRU_predicted = model_gru.predict(X_test)

GRU_predicted_stock_price = sc.inverse_transform(GRU_predicted)

# 可视化
plot_predictions(test_set, GRU_predicted_stock_price)

'''