import { getAllKfConfig, setKfConfig, removeKfConfig } from '__gUtils/kungfuUtils';
import { logger } from '../../utils/logUtils';


export const getTdList = (): Promise<Td[]> => {
    return new Promise(resolve => {
        const kfConfigList = getAllKfConfig();
        const tdList: Td[] = Object.values(kfConfigList || {})
            .filter((tdData: any) => {
                return +tdData.category === 1
            })
            .map((tdData: any) => {
                return {
                    account_id: `${tdData.group}_${tdData.name}`,
                    source_name: tdData.group.toString(),
                    config: tdData.value
                }
            })
            .sort((tdData1: any, tdData2: any) => {
                if (tdData1.account_id > tdData2.account_id) {
                    return 1
                } else if (tdData1.account_id < tdData2.account_id) {
                    return -1
                } else {
                    return 0
                }
            })

        resolve(tdList)
    })
}

export const getMdList = (): Promise<Md[]> => {
    return new Promise(resolve => {
        const kfConfigList = getAllKfConfig();
        const mdList = Object.values(kfConfigList || {})
            .filter((mdData: any) => {
                return +mdData.category === 0
            })
            .map((mdData: any) => {
                return {
                    source_name: mdData.group,
                    config: mdData.value
                }
            })
            .sort((mdData1: any, mdData2: any) => {
                if (mdData1.source_name > mdData2.source_name) {
                    return 1
                } else if (mdData1.source_name < mdData2.source_name) {
                    return -1
                } else {
                    return 0
                }

            })
            
        resolve(mdList)
    })
}

export const addTd = (accountId: string, config: string) => {
    return new Promise(resolve => {
        const td = setKfConfig(accountId, 'td', config);
        resolve(td)
    })
}

export const addMd = (sourceId: string, config: string) => {
    return new Promise(resolve => {
        const md = setKfConfig(sourceId, 'md', config);
        resolve(md)
    })
}

export const updateTdConfig = (accountId: string, config: string) => {
    return new Promise(resolve => {
        const td = setKfConfig(accountId, 'td', config)
        resolve(td)
    })
}

export const updateMdConfig = (sourceName: string, config: string) => {
    return new Promise(resolve => {
        const md = setKfConfig(sourceName, 'md', config)
        resolve(md)
    })
}

export const deleteTd = (accountId: string) => {
    return new Promise(resolve => {
        const td = removeKfConfig(accountId, 'td');
        resolve(td)
    })
}

export const deleteMd = (sourceName: string) => {
    return new Promise(resolve => {
        const md = removeKfConfig(sourceName, 'md');
        resolve(md)
    })
}

export const getExistedMdSources = () => getMdList()
    .then((mdList: any) => {
        return mdList.map((m: any) => m.source_name)
    })